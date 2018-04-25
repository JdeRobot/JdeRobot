#!/usr/bin/env python
'''
mavproxy - a MAVLink proxy program

Copyright Andrew Tridgell 2011
Released under the GNU GPL version 3 or later

'''

import sys, os, time, socket, signal
import fnmatch, errno, threading
import serial, select
import Queue
import imp
import traceback
import select
import shlex
import math
import Ice
import jderobot
import multiprocessing
import time
import sys
import config
import comm

from MAVProxy.modules.lib import textconsole
from MAVProxy.modules.lib import rline
from MAVProxy.modules.lib import mp_module
from MAVProxy.modules.lib import dumpstacks
from MAVProxy.modules.lib import udp
from MAVProxy.modules.lib import tcp

import easyiceconfig as EasyIce

from Pose3D import Pose3DI
from CMDVel import CMDVelI
from Extra import ExtraI
from pymavlink import quaternion

global operation_takeoff
global time_init_operation_takeoff
global time_end_operation_takeoff
global on_air

# adding all this allows pyinstaller to build a working windows executable
# note that using --hidden-import does not work for these modules
try:
      from multiprocessing import freeze_support
      from pymavlink import mavwp, mavutil
      import matplotlib, HTMLParser
      try:
            import readline
      except ImportError:
            import pyreadline as readline
except Exception:
      pass


if __name__ == '__main__':
      freeze_support()

class MPStatus(object):
    '''hold status information about the mavproxy'''
    def __init__(self):
        self.gps	 = None
        self.msgs = {}
        self.msg_count = {}
        self.counters = {'MasterIn' : [], 'MasterOut' : 0, 'FGearIn' : 0, 'FGearOut' : 0, 'Slave' : 0}
        self.setup_mode = opts.setup
        self.mav_error = 0
        self.altitude = 0
        self.last_altitude_announce = 0.0
        self.last_distance_announce = 0.0
        self.exit = False
        self.flightmode = 'MAV'
        self.last_mode_announce = 0
        self.logdir = None
        self.last_heartbeat = 0
        self.last_message = 0
        self.heartbeat_error = False
        self.last_apm_msg = None
        self.last_apm_msg_time = 0
        self.highest_msec = 0
        self.have_gps_lock = False
        self.lost_gps_lock = False
        self.last_gps_lock = 0
        self.watch = None
        self.last_streamrate1 = -1
        self.last_streamrate2 = -1
        self.last_seq = 0
        self.armed = False

    def show(self, f, pattern=None):
        '''write status to status.txt'''
        if pattern is None:
            f.write('Counters: ')
            for c in self.counters:
                f.write('%s:%s ' % (c, self.counters[c]))
            f.write('\n')
            f.write('MAV Errors: %u\n' % self.mav_error)
            f.write(str(self.gps)+'\n')
        for m in sorted(self.msgs.keys()):
            if pattern is not None and not fnmatch.fnmatch(str(m).upper(), pattern.upper()):
                continue
            f.write("%u: %s\n" % (self.msg_count[m], str(self.msgs[m])))

    def write(self):
        '''write status to status.txt'''
        f = open('status.txt', mode='w')
        self.show(f)
        f.close()

def say_text(text, priority='important'):
    '''text output - default function for say()'''
    mpstate.console.writeln(text)

def say(text, priority='important'):
    '''text and/or speech output'''
    mpstate.functions.say(text, priority)

def add_input(cmd, immediate=False):
    '''add some command input to be processed'''
    if immediate:
        process_stdin(cmd)
    else:
        mpstate.input_queue.put(cmd)

class MAVFunctions(object):
    '''core functions available in modules'''
    def __init__(self):
        self.process_stdin = add_input
        self.param_set = param_set
        self.get_mav_param = get_mav_param
        self.say = say_text
        # input handler can be overridden by a module
        self.input_handler = None

class MPState(object):
    '''holds state of mavproxy'''
    def __init__(self):
        self.udp = udp.UdpServer()
        self.tcp = tcp.TcpServer()
        self.console = textconsole.SimpleConsole(udp = self.udp, tcp = self.tcp)
        self.map = None
        self.map_functions = {}
        self.vehicle_type = None
        self.vehicle_name = None
        from MAVProxy.modules.lib.mp_settings import MPSettings, MPSetting
        self.settings = MPSettings(
            [ MPSetting('link', int, 1, 'Primary Link', tab='Link', range=(0,4), increment=1),
              MPSetting('streamrate', int, 4, 'Stream rate link1', range=(-1,20), increment=1),
              MPSetting('streamrate2', int, 4, 'Stream rate link2', range=(-1,20), increment=1),
              MPSetting('heartbeat', int, 1, 'Heartbeat rate', range=(0,5), increment=1),
              MPSetting('mavfwd', bool, True, 'Allow forwarded control'),
              MPSetting('mavfwd_rate', bool, False, 'Allow forwarded rate control'),
              MPSetting('shownoise', bool, True, 'Show non-MAVLink data'),
              MPSetting('baudrate', int, opts.baudrate, 'baudrate for new links', range=(0,10000000), increment=1),
              MPSetting('rtscts', bool, opts.rtscts, 'enable flow control'),
              MPSetting('select_timeout', float, 0.01, 'select timeout'),

              MPSetting('altreadout', int, 10, 'Altitude Readout',
                        range=(0,100), increment=1, tab='Announcements'),
              MPSetting('distreadout', int, 200, 'Distance Readout', range=(0,10000), increment=1),

              MPSetting('moddebug', int, opts.moddebug, 'Module Debug Level', range=(0,3), increment=1, tab='Debug'),
              MPSetting('compdebug', int, 0, 'Computation Debug Mask', range=(0,3), tab='Debug'),
              MPSetting('flushlogs', bool, False, 'Flush logs on every packet'),
              MPSetting('requireexit', bool, False, 'Require exit command'),
              MPSetting('wpupdates', bool, True, 'Announce waypoint updates'),

              MPSetting('basealt', int, 0, 'Base Altitude', range=(0,30000), increment=1, tab='Altitude'),
              MPSetting('wpalt', int, 100, 'Default WP Altitude', range=(0,10000), increment=1),
              MPSetting('rallyalt', int, 90, 'Default Rally Altitude', range=(0,10000), increment=1),
              MPSetting('terrainalt', str, 'Auto', 'Use terrain altitudes', choice=['Auto','True','False']),
              MPSetting('rally_breakalt', int, 40, 'Default Rally Break Altitude', range=(0,10000), increment=1),
              MPSetting('rally_flags', int, 0, 'Default Rally Flags', range=(0,10000), increment=1),

              MPSetting('source_system', int, 255, 'MAVLink Source system', range=(0,255), increment=1, tab='MAVLink'),
              MPSetting('source_component', int, 0, 'MAVLink Source component', range=(0,255), increment=1),
              MPSetting('target_system', int, 0, 'MAVLink target system', range=(0,255), increment=1),
              MPSetting('target_component', int, 0, 'MAVLink target component', range=(0,255), increment=1),
              MPSetting('state_basedir', str, None, 'base directory for logs and aircraft directories')
            ])

        self.completions = {
            "script"         : ["(FILENAME)"],
            "set"            : ["(SETTING)"],
            "status"         : ["(VARIABLE)"],
            "module"    : ["list",
                           "load (AVAILMODULES)",
                           "<unload|reload> (LOADEDMODULES)"]
            }

        self.status = MPStatus()

        # master mavlink device
        self.mav_master = None

        # mavlink outputs
        self.mav_outputs = []
        self.sysid_outputs = {}

        # SITL output
        self.sitl_output = None

        self.mav_param = mavparm.MAVParmDict()
        self.modules = []
        self.public_modules = {}
        self.functions = MAVFunctions()
        self.select_extra = {}
        self.continue_mode = False
        self.aliases = {}
        import platform
        self.system = platform.system()

    def module(self, name):
        '''Find a public module (most modules are private)'''
        if name in self.public_modules:
            return self.public_modules[name]
        return None

    def master(self):
        '''return the currently chosen mavlink master object'''
        if len(self.mav_master) == 0:
              return None
        if self.settings.link > len(self.mav_master):
            self.settings.link = 1
        # try to use one with no link error
        if not self.mav_master[self.settings.link-1].linkerror:
            return self.mav_master[self.settings.link-1]
        for m in self.mav_master:
            if not m.linkerror:
                return m
        return self.mav_master[self.settings.link-1]


def get_mav_param(param, default=None):
    '''return a EEPROM parameter value'''
    return mpstate.mav_param.get(param, default)

def param_set(name, value, retries=3):
    '''set a parameter'''
    name = name.upper()
    return mpstate.mav_param.mavset(mpstate.master(), name, value, retries=retries)

def cmd_script(args):
    '''run a script'''
    if len(args) < 1:
        print("usage: script <filename>")
        return

    run_script(args[0])

def cmd_set(args):
    '''control mavproxy options'''
    mpstate.settings.command(args)

def cmd_status(args):
    '''show status'''
    if len(args) == 0:
        mpstate.status.show(sys.stdout, pattern=None)
    else:
        for pattern in args:
            mpstate.status.show(sys.stdout, pattern=pattern)

def cmd_setup(args):
    mpstate.status.setup_mode = True
    mpstate.rl.set_prompt("")


def cmd_reset(args):
    print("Resetting master")
    mpstate.master().reset()

def cmd_watch(args):
    '''watch a mavlink packet pattern'''
    if len(args) == 0:
        mpstate.status.watch = None
        return
    mpstate.status.watch = args[0]
    print("Watching %s" % mpstate.status.watch)

def load_module(modname, quiet=False):
    '''load a module'''
    modpaths = ['MAVProxy.modules.mavproxy_%s' % modname, modname]
    for (m,pm) in mpstate.modules:
        if m.name == modname:
            if not quiet:
                print("module %s already loaded" % modname)
            return False
    for modpath in modpaths:
        try:
            m = import_package(modpath)
            imp.reload(m)
            module = m.init(mpstate)
            if isinstance(module, mp_module.MPModule):
                mpstate.modules.append((module, m))
                if not quiet:
                    print("Loaded module %s" % (modname,))
                return True
            else:
                ex = "%s.init did not return a MPModule instance" % modname
                break
        except ImportError as msg:
            ex = msg
            if mpstate.settings.moddebug > 1:
                import traceback
                print(traceback.format_exc())
    print("Failed to load module: %s. Use 'set moddebug 3' in the MAVProxy console to enable traceback" % ex)
    return False

def unload_module(modname):
    '''unload a module'''
    for (m,pm) in mpstate.modules:
        if m.name == modname:
            if hasattr(m, 'unload'):
                m.unload()
            mpstate.modules.remove((m,pm))
            print("Unloaded module %s" % modname)
            return True
    print("Unable to find module %s" % modname)
    return False

def cmd_module(args):
    '''module commands'''
    usage = "usage: module <list|load|reload|unload>"
    if len(args) < 1:
        print(usage)
        return
    if args[0] == "list":
        for (m,pm) in mpstate.modules:
            print("%s: %s" % (m.name, m.description))
    elif args[0] == "load":
        if len(args) < 2:
            print("usage: module load <name>")
            return
        load_module(args[1])
    elif args[0] == "reload":
        if len(args) < 2:
            print("usage: module reload <name>")
            return
        modname = args[1]
        pmodule = None
        for (m,pm) in mpstate.modules:
            if m.name == modname:
                pmodule = pm
        if pmodule is None:
            print("Module %s not loaded" % modname)
            return
        if unload_module(modname):
            import zipimport
            try:
                reload(pmodule)
            except ImportError:
                clear_zipimport_cache()
                reload(pmodule)
            if load_module(modname, quiet=True):
                print("Reloaded module %s" % modname)
    elif args[0] == "unload":
        if len(args) < 2:
            print("usage: module unload <name>")
            return
        modname = os.path.basename(args[1])
        unload_module(modname)
    else:
        print(usage)


def cmd_alias(args):
    '''alias commands'''
    usage = "usage: alias <add|remove|list>"
    if len(args) < 1 or args[0] == "list":
        if len(args) >= 2:
            wildcard = args[1].upper()
        else:
            wildcard = '*'
        for a in sorted(mpstate.aliases.keys()):
            if fnmatch.fnmatch(a.upper(), wildcard):
                print("%-15s : %s" % (a, mpstate.aliases[a]))
    elif args[0] == "add":
        if len(args) < 3:
            print(usage)
            return
        a = args[1]
        mpstate.aliases[a] = ' '.join(args[2:])
    elif args[0] == "remove":
        if len(args) != 2:
            print(usage)
            return
        a = args[1]
        if a in mpstate.aliases:
            mpstate.aliases.pop(a)
        else:
            print("no alias %s" % a)
    else:
        print(usage)
        return


def clear_zipimport_cache():
    """Clear out cached entries from _zip_directory_cache.
    See http://www.digi.com/wiki/developer/index.php/Error_messages"""
    import sys, zipimport
    syspath_backup = list(sys.path)
    zipimport._zip_directory_cache.clear()

    # load back items onto sys.path
    sys.path = syspath_backup
    # add this too: see https://mail.python.org/pipermail/python-list/2005-May/353229.html
    sys.path_importer_cache.clear()

# http://stackoverflow.com/questions/211100/pythons-import-doesnt-work-as-expected
# has info on why this is necessary.

def import_package(name):
    """Given a package name like 'foo.bar.quux', imports the package
    and returns the desired module."""
    import zipimport
    try:
        mod = __import__(name)
    except ImportError:
        clear_zipimport_cache()
        mod = __import__(name)

    components = name.split('.')
    for comp in components[1:]:
        mod = getattr(mod, comp)
    return mod


command_map = {
    'script'  : (cmd_script,   'run a script of MAVProxy commands'),
    'setup'   : (cmd_setup,    'go into setup mode'),
    'reset'   : (cmd_reset,    'reopen the connection to the MAVLink master'),
    'status'  : (cmd_status,   'show status'),
    'set'     : (cmd_set,      'mavproxy settings'),
    'watch'   : (cmd_watch,    'watch a MAVLink pattern'),
    'module'  : (cmd_module,   'module commands'),
    'alias'   : (cmd_alias,    'command aliases')
    }

def process_stdin(line):
    '''handle commands from user'''
    if line is None:
        sys.exit(0)
    # allow for modules to override input handling
    if mpstate.functions.input_handler is not None:
          mpstate.functions.input_handler(line)
          return

    line = line.strip()

    if mpstate.status.setup_mode:
        # in setup mode we send strings straight to the master
        if line == '.':
            mpstate.status.setup_mode = False
            mpstate.status.flightmode = "MAV"
            mpstate.rl.set_prompt("MAV> ")
            return
        if line != '+++':
            line += '\r'
        for c in line:
            time.sleep(0.01)
            mpstate.master().write(c)
        return

    if not line:
        return

    args = shlex.split(line)
    cmd = args[0]
    while cmd in mpstate.aliases:
        line = mpstate.aliases[cmd]
        args = shlex.split(line) + args[1:]
        cmd = args[0]

    if cmd == 'help':
        k = command_map.keys()
        #k.sort()
        for cmd in k:
            (fn, help) = command_map[cmd]
            print("%-15s : %s" % (cmd, help))
        return
    if cmd == 'exit' and mpstate.settings.requireexit:
        mpstate.status.exit = True
        return
    ######################################################################################################
    if cmd == 'velocity' and len(args) == 4:
        PH_CMDVel = CMDVelI(args[1],args[2],args[3],0,0,0) #1 to avoid indeterminations






    ######################################################################################################
    if not cmd in command_map:
        for (m,pm) in mpstate.modules:
            if hasattr(m, 'unknown_command'):
                try:
                    if m.unknown_command(args):
                        return
                except Exception as e:
                    print("ERROR in command: %s" % str(e))
        print("Unknown command '%s'" % line)
        return

    (fn, help) = command_map[cmd]
    try:
        fn(args[1:])
    except Exception as e:
        print("ERROR in command %s: %s" % (args[1:], str(e)))
        if mpstate.settings.moddebug > 1:
            traceback.print_exc()


def process_master(m):
    '''process packets from the MAVLink master'''
    try:
        s = m.recv(16*1024)
    except Exception:
        time.sleep(0.1)
        return
    # prevent a dead serial port from causing the CPU to spin. The user hitting enter will
    # cause it to try and reconnect
    if len(s) == 0:
        time.sleep(0.1)
        return

    if (mpstate.settings.compdebug & 1) != 0:
        return

    if mpstate.logqueue_raw:
        mpstate.logqueue_raw.put(str(s))

    if mpstate.status.setup_mode:
        if mpstate.system == 'Windows':
           # strip nsh ansi codes
           s = s.replace("\033[K","")
        sys.stdout.write(str(s))
        sys.stdout.flush()
        return

    if m.first_byte and opts.auto_protocol:
        m.auto_mavlink_version(s)
    msgs = m.mav.parse_buffer(s)
    if msgs:
        for msg in msgs:
            sysid = msg.get_srcSystem()
            if sysid in mpstate.sysid_outputs:
                  # the message has been handled by a specialised handler for this system
                  continue
            if getattr(m, '_timestamp', None) is None:
                m.post_message(msg)
            if msg.get_type() == "BAD_DATA":
                if opts.show_errors:
                    mpstate.console.writeln("MAV error: %s" % msg)
                mpstate.status.mav_error += 1



def process_mavlink(slave):
    '''process packets from MAVLink slaves, forwarding to the master'''
    try:
        buf = slave.recv()
    except socket.error:
        return
    try:
        if slave.first_byte and opts.auto_protocol:
            slave.auto_mavlink_version(buf)
        msgs = slave.mav.parse_buffer(buf)
    except mavutil.mavlink.MAVError as e:
        mpstate.console.error("Bad MAVLink slave message from %s: %s" % (slave.address, e.message))
        return
    if msgs is None:
        return
    if mpstate.settings.mavfwd and not mpstate.status.setup_mode:
        for m in msgs:
            if mpstate.status.watch is not None:
                if fnmatch.fnmatch(m.get_type().upper(), mpstate.status.watch.upper()):
                    mpstate.console.writeln('> '+ str(m))
            mpstate.master().write(m.get_msgbuf())
    mpstate.status.counters['Slave'] += 1


def mkdir_p(dir):
    '''like mkdir -p'''
    if not dir:
        return
    if dir.endswith("/"):
        mkdir_p(dir[:-1])
        return
    if os.path.isdir(dir):
        return
    mkdir_p(os.path.dirname(dir))
    os.mkdir(dir)

def log_writer():
    '''log writing thread'''
    while True:
        mpstate.logfile_raw.write(mpstate.logqueue_raw.get())
        while not mpstate.logqueue_raw.empty():
            mpstate.logfile_raw.write(mpstate.logqueue_raw.get())
        while not mpstate.logqueue.empty():
            mpstate.logfile.write(mpstate.logqueue.get())
        if mpstate.settings.flushlogs:
            mpstate.logfile.flush()
            mpstate.logfile_raw.flush()

# If state_basedir is NOT set then paths for logs and aircraft
# directories are relative to mavproxy's cwd
def log_paths():
    '''Returns tuple (logdir, telemetry_log_filepath, raw_telemetry_log_filepath)'''
    if opts.aircraft is not None:
        if opts.mission is not None:
            print(opts.mission)
            dirname = "%s/logs/%s/Mission%s" % (opts.aircraft, time.strftime("%Y-%m-%d"), opts.mission)
        else:
            dirname = "%s/logs/%s" % (opts.aircraft, time.strftime("%Y-%m-%d"))
        # dirname is currently relative.  Possibly add state_basedir:
        if mpstate.settings.state_basedir is not None:
            dirname = os.path.join(mpstate.settings.state_basedir,dirname)
        mkdir_p(dirname)
        highest = None
        for i in range(1, 10000):
            fdir = os.path.join(dirname, 'flight%u' % i)
            if not os.path.exists(fdir):
                break
            highest = fdir
        if mpstate.continue_mode and highest is not None:
            fdir = highest
        elif os.path.exists(fdir):
            print("Flight logs full")
            sys.exit(1)
        logname = 'flight.tlog'
        logdir = fdir
    else:
        logname = os.path.basename(opts.logfile)
        dir_path = os.path.dirname(opts.logfile)
        if not os.path.isabs(dir_path) and mpstate.settings.state_basedir is not None:
            dir_path = os.path.join(mpstate.settings.state_basedir,dir_path)
        logdir = dir_path

    mkdir_p(logdir)
    return (logdir,
            os.path.join(logdir, logname),
            os.path.join(logdir, logname + '.raw'))


def open_telemetry_logs(logpath_telem, logpath_telem_raw):
    '''open log files'''
    if opts.append_log or opts.continue_mode:
        mode = 'a'
    else:
        mode = 'w'
    mpstate.logfile = open(logpath_telem, mode=mode)
    mpstate.logfile_raw = open(logpath_telem_raw, mode=mode)
    print("Log Directory: %s" % mpstate.status.logdir)
    print("Telemetry log: %s" % logpath_telem)

    # use a separate thread for writing to the logfile to prevent
    # delays during disk writes (important as delays can be long if camera
    # app is running)
    t = threading.Thread(target=log_writer, name='log_writer')
    t.daemon = True
    t.start()

def set_stream_rates():
    '''set mavlink stream rates'''
    if (not msg_period.trigger() and
        mpstate.status.last_streamrate1 == mpstate.settings.streamrate and
        mpstate.status.last_streamrate2 == mpstate.settings.streamrate2):
        return
    mpstate.status.last_streamrate1 = mpstate.settings.streamrate
    mpstate.status.last_streamrate2 = mpstate.settings.streamrate2
    for master in mpstate.mav_master:
        if master.linknum == 0:
            rate = mpstate.settings.streamrate
        else:
            rate = mpstate.settings.streamrate2
        if rate != -1:
            master.mav.request_data_stream_send(mpstate.settings.target_system, mpstate.settings.target_component,
                                                mavutil.mavlink.MAV_DATA_STREAM_ALL,
                                                rate, 1)

def check_link_status():
    '''check status of master links'''
    tnow = time.time()
    if mpstate.status.last_message != 0 and tnow > mpstate.status.last_message + 5:
        say("no link")
        mpstate.status.heartbeat_error = True
    for master in mpstate.mav_master:
        if not master.linkerror and (tnow > master.last_message + 5 or master.portdead):
            say("link %u down" % (master.linknum+1))
            master.linkerror = True

def send_heartbeat(master):
    if master.mavlink10():
        master.mav.heartbeat_send(mavutil.mavlink.MAV_TYPE_GCS, mavutil.mavlink.MAV_AUTOPILOT_INVALID,
                                  0, 0, 0)
    else:
        MAV_GROUND = 5
        MAV_AUTOPILOT_NONE = 4
        master.mav.heartbeat_send(MAV_GROUND, MAV_AUTOPILOT_NONE)

def periodic_tasks():
    '''run periodic checks'''
    if mpstate.status.setup_mode:
        return

    if (mpstate.settings.compdebug & 2) != 0:
        return

    if mpstate.settings.heartbeat != 0:
        heartbeat_period.frequency = mpstate.settings.heartbeat

    if heartbeat_period.trigger() and mpstate.settings.heartbeat != 0:
        mpstate.status.counters['MasterOut'] += 1
        for master in mpstate.mav_master:
            send_heartbeat(master)

    if heartbeat_check_period.trigger():
        check_link_status()

    set_stream_rates()

    # call optional module idle tasks. These are called at several hundred Hz
    for (m,pm) in mpstate.modules:
        if hasattr(m, 'idle_task'):
            try:
                m.idle_task()
            except Exception as msg:
                if mpstate.settings.moddebug == 1:
                    print(msg)
                elif mpstate.settings.moddebug > 1:
                    exc_type, exc_value, exc_traceback = sys.exc_info()
                    traceback.print_exception(exc_type, exc_value, exc_traceback,
                                              limit=2, file=sys.stdout)

        # also see if the module should be unloaded:
        if m.needs_unloading:
            unload_module(m.name)

def main_loop():

    if not mpstate.status.setup_mode and not opts.nowait:
        for master in mpstate.mav_master:
            send_heartbeat(master)
            if master.linknum == 0:
                print("Waiting for heartbeat from %s" % master.address)
                master.wait_heartbeat()
        set_stream_rates()

    while True:
        if mpstate is None or mpstate.status.exit:
            return

        global on_air
        global operation_takeoff
        global time_init_operation_takeoff
        global time_end_operation_takeoff

        time_now = int(round(time.time() * 1000))

        if operation_takeoff  and time_now > time_end_operation_takeoff:
            print("Taking off")
            time_init_operation_takeoff = int(round(time.time() * 1000))
            time_end_operation_takeoff = time_init_operation_takeoff + 5000
            operation_takeoff = False
            on_air = True
            mpstate.input_queue.put("takeoff 1")

        if on_air and time_now > time_end_operation_takeoff:
            mpstate.input_queue.put("mode guided")
            print("Mode guided on")
            on_air = False

        while not mpstate.input_queue.empty():
            line = mpstate.input_queue.get()
            mpstate.input_count += 1
            cmds = line.split(';')
            if len(cmds) == 1 and cmds[0] == "":
                mpstate.empty_input_count += 1
            for c in cmds:
                #print(c)
                process_stdin(c)

        for master in mpstate.mav_master:
            if master.fd is None:
                if master.port.inWaiting() > 0:
                    process_master(master)

        periodic_tasks()

        rin = []
        for master in mpstate.mav_master:
            if master.fd is not None and not master.portdead:
                rin.append(master.fd)
        for m in mpstate.mav_outputs:
            rin.append(m.fd)
        for sysid in mpstate.sysid_outputs:
            m = mpstate.sysid_outputs[sysid]
            rin.append(m.fd)
        if rin == []:
            time.sleep(0.0001)
            continue

        for fd in mpstate.select_extra:
            rin.append(fd)
        try:
            (rin, win, xin) = select.select(rin, [], [], mpstate.settings.select_timeout)
        except select.error:
            continue

        if mpstate is None:
            return

        for fd in rin:
            if mpstate is None:
                return
            for master in mpstate.mav_master:
                if fd == master.fd:
                    process_master(master)
                    if mpstate is None:
                          return
                    continue
            for m in mpstate.mav_outputs:
                if fd == m.fd:
                    process_mavlink(m)
                    if mpstate is None:
                        return
                    continue

            for sysid in mpstate.sysid_outputs:
                m = mpstate.sysid_outputs[sysid]
                if fd == m.fd:
                    process_mavlink(m)
                    if mpstate is None:
                        return
                    continue

        # this allow modules to register their own file descriptors
        # for the main select loop
            if fd in mpstate.select_extra:
                try:
                # call the registered read function
                    (fn, args) = mpstate.select_extra[fd]
                    fn(args)
                except Exception as msg:
                    if mpstate.settings.moddebug == 1:
                        print(msg)
                # on an exception, remove it from the select list
                    mpstate.select_extra.pop(fd)

        ########################## Jorge Cano CODE ##########################

            Rollvalue = mpstate.status.msgs['ATTITUDE'].roll    #rad
            Pitchvalue = mpstate.status.msgs['ATTITUDE'].pitch  #rad
            Yawvalue = mpstate.status.msgs['ATTITUDE'].yaw      #rad

        # ESTIMATED: fused GPS and accelerometers
            PoseLatLonHei = {}
            PoseLatLonHei['lat'] = math.radians((mpstate.status.msgs['GLOBAL_POSITION_INT'].lat)/1E7)    #rad
            PoseLatLonHei['lon'] = math.radians((mpstate.status.msgs['GLOBAL_POSITION_INT'].lon)/1E7)    #rad
            PoseLatLonHei['hei'] = (mpstate.status.msgs['GLOBAL_POSITION_INT'].relative_alt)/1000   #meters

            PH_quat = quaternion.Quaternion([Rollvalue, Pitchvalue, Yawvalue])
            PH_xyz = global2cartesian(PoseLatLonHei)

            #print (PH_quat)
            #print (PH_xyz)

            data = jderobot.Pose3DData()
            data.x = PH_xyz['x']
            data.y = PH_xyz['y']
            data.z = PH_xyz['z']
            data.h = 1
            data.q0 = PH_quat.__getitem__(0)
            data.q1 = PH_quat.__getitem__(1)
            data.q2 = PH_quat.__getitem__(2)
            data.q3 = PH_quat.__getitem__(3)
            #print(data)
            PH_Pose3D.setPose3DData(data)

        #####################################################################

def input_loop():
    '''wait for user input'''
    global operation_takeoff
    global time_init_operation_takeoff
    global time_end_operation_takeoff

    while mpstate.status.exit != True:
        try:
            if mpstate.status.exit != True:
                if mpstate.udp.bound():
                    line = mpstate.udp.readln()
                    mpstate.udp.writeln(line)
                elif mpstate.tcp.connected():
                    line = mpstate.tcp.readln()
                    mpstate.tcp.writeln(line)
                else:
                    line = input(mpstate.rl.prompt)
            if line == 'takeoff':
                print("Detecto takeoff")
                operation_takeoff=True
                time_init_operation_takeoff = int(round(time.time() * 1000))
                time_end_operation_takeoff = time_init_operation_takeoff + 5000
                print(time_end_operation_takeoff)
                mpstate.input_queue.put("arm throttle")
                return
            if line == 'land':
                print("Orden de aterrizar")
                on_air = False
        except EOFError:
            mpstate.status.exit = True
            sys.exit(1)
        mpstate.input_queue.put(line)

def run_script(scriptfile):
    '''run a script file'''
    try:
        f = open(scriptfile, mode='r')
    except Exception:
        return
    mpstate.console.writeln("Running script %s" % scriptfile)
    for line in f:
        line = line.strip()
        if line == "" or line.startswith('#'):
            continue
        if line.startswith('@'):
            line = line[1:]
        else:
            mpstate.console.writeln("-> %s" % line)
        process_stdin(line)
    f.close()

########################## Jorge Cano CODE ##########################

def openPose3DChannel(Pose3D,cfg):
    status = 0
    ic = None
    Pose2Tx = Pose3D #Pose3D.getPose3DData()
    try:
        id = Ice.InitializationData()
        ic = Ice.initialize(None, id)

        endpoint = cfg.getProperty("Pose3D.Proxy")
        name = cfg.getProperty("Pose3D.Name")

        adapter = ic.createObjectAdapterWithEndpoints("Pose3D", endpoint)

        object = Pose2Tx
        adapter.add(object, ic.stringToIdentity("Pose3D"))
        adapter.activate()
        ic.waitForShutdown()
    except:
        traceback.print_exc()
        status = 1

    if ic:
        # Clean up
        try:
            ic.destroy()
        except:
            traceback.print_exc()
            status = 1

    sys.exit(status)

def openPose3DChannelWP(Pose3D,cfg):

    status = 0
    ic = None
    Pose2Rx = Pose3D #Pose3D.getPose3DData()
    try:
        id = Ice.InitializationData()
        ic = Ice.initialize(None, id)

        endpoint = cfg.getProperty("Pose3D.Proxy")
        name = cfg.getProperty("Pose3D.Name")

        adapter = ic.createObjectAdapterWithEndpoints("Pose3DAdapter", endpoint)

        object = Pose2Rx
        adapter.add(object, ic.stringToIdentity("Pose3DAdapter"))
        adapter.activate()
        ic.waitForShutdown()
    except:
        traceback.print_exc()
        status = 1

    if ic:
        # Clean up
        try:
            ic.destroy()
        except:
            traceback.print_exc()
            status = 1

    sys.exit(status)

def openCMDVelChannel(CMDVel,cfg):
    status = 0
    ic = None
    CMDVel2Rx = CMDVel #CMDVel.getCMDVelData()
    try:
        id = Ice.InitializationData()
        ic = Ice.initialize(None, id)

        endpoint = cfg.getProperty("CMDVel.Proxy")
        name = cfg.getProperty("CMDVel.Name")

        adapter = ic.createObjectAdapterWithEndpoints("CMDVel", endpoint)

        object = CMDVel2Rx
        adapter.add(object, ic.stringToIdentity("CMDVel"))
        adapter.activate()
        ic.waitForShutdown()
    except:
        traceback.print_exc()
        status = 1

    if ic:
        # Clean up
        try:
            ic.destroy()
        except:
            traceback.print_exc()
            status = 1

    sys.exit(status)

def openExtraChannel(Extra,cfg):

    status = 0
    ic = None
    Extra2Tx = Extra
    try:
        id = Ice.InitializationData()
        ic = Ice.initialize(None, id)

        endpoint = cfg.getProperty("Extra.Proxy")
        name = cfg.getProperty("Extra.Name")

        adapter = ic.createObjectAdapterWithEndpoints("Extra", endpoint)

        object = Extra2Tx
        adapter.add(object, ic.stringToIdentity("Extra"))
        adapter.activate()
        ic.waitForShutdown()
    except:
        traceback.print_exc()
        status = 1

    if ic:
        # Clean up
        try:
            ic.destroy()
        except:
            traceback.print_exc()
            status = 1

    sys.exit(status)

class NavdataI(jderobot.Navdata):
    def __init__(self):
        pass

    def getNavdata(self, current=None):
        data = jderobot.NavdataData()
        return data

def openNavdataChannel(cfg):

    status = 0
    ic = None
    Navdata2Tx = NavdataI()

    try:

        id = Ice.InitializationData()
        ic = Ice.initialize(None, id)

        endpoint = cfg.getProperty("Navdata.Proxy")
        name = cfg.getProperty("Navdata.Name")

        adapter = ic.createObjectAdapterWithEndpoints("Navdata", endpoint)

        object = Navdata2Tx
        adapter.add(object, ic.stringToIdentity("Navdata"))
        adapter.activate()
        ic.waitForShutdown()
    except:
        traceback.print_exc()
        status = 1

    if ic:
        # Clean up
        try:
            ic.destroy()
        except:
            traceback.print_exc()
            status = 1

    sys.exit(status)

def sendCMDVel2Vehicle(CMDVel,Pose3D):
    absolute = 0
    relative = 1

    while True:

        CMDVel2send = CMDVel.getCMDVelData()
        Pose3D2send = Pose3D.getPose3DData()
        #print(Pose3D2send)
        NEDvel = body2NED(CMDVel2send, Pose3D2send) # [x,y,z]
        linearXstring = str(NEDvel[0])
        linearYstring = str(NEDvel[1])
        linearZstring = str(NEDvel[2])

        #CMDVel.angularZ -1 y 1

        angular = CMDVel.angularZ

        if angular >= 0:
            direction = str(1)
        else:
            angular = -angular
            direction = str(-1)

        angularZstring = str(angular*30)

        movement = str(relative)

        velocitystring = 'velocity '+ linearXstring + ' ' + linearYstring + ' ' + linearZstring
        angularString = 'setyaw ' + angularZstring + ' ' + direction + ' ' + movement

        process_stdin(velocitystring)  # SET_POSITION_TARGET_LOCAL_NED
        process_stdin(angularString)

def sendWayPoint2Vehicle(Pose3D):

    while True:
        time.sleep(1)
        wayPointPoseXYZ = Pose3D.getPose3DData()
        wayPointXYZ = {}
        wayPointXYZ['x'] = wayPointPoseXYZ.x
        wayPointXYZ['y'] = wayPointPoseXYZ.y
        wayPointXYZ['z'] = wayPointPoseXYZ.z
        wayPointLatLonHei =  cartesian2global(wayPointXYZ)

        latittude = str(wayPointLatLonHei['lat'])
        longitude = str(wayPointLatLonHei['lon'])
        altittude = str(int(wayPointLatLonHei['hei']))

        WPstring = 'guided ' + latittude + ' ' + longitude + ' ' + altittude
        process_stdin(WPstring)

        #print wayPoint

def landDecision(PH_Extra):

    global operation_takeoff
    global time_init_operation_takeoff
    global time_end_operation_takeoff
    while True:
        if PH_Extra.landDecision:
            print("Landing")
            process_stdin("land")
            PH_Extra.setLand(False)
        if PH_Extra.takeOffDecision:
            print("Takeoff")
            operation_takeoff=True
            print(time_end_operation_takeoff)
            time_init_operation_takeoff = int(round(time.time() * 1000))
            time_end_operation_takeoff = time_init_operation_takeoff + 5000
            print("Arming proppellers")
            mpstate.input_queue.put("arm throttle")
            PH_Extra.setTakeOff(False)

def global2cartesian(poseLatLonHei):

    wgs84_radius = 6378137 #meters
    wgs84_flattening = 1 - 1 / 298.257223563
    eartPerim = wgs84_radius * 2 * math.pi

    earthRadiusLon = wgs84_radius * math.cos(poseLatLonHei['lat'])/wgs84_flattening
    eartPerimLon = earthRadiusLon * 2 * math.pi

    poseXYZ = {}
    poseXYZ['x'] = poseLatLonHei['lon'] * eartPerimLon / (2*math.pi)
    poseXYZ['y'] = poseLatLonHei['lat'] * eartPerim / (2*math.pi)
    poseXYZ['z'] = poseLatLonHei['hei']

    return poseXYZ

def cartesian2global(poseXYZ):

    wgs84_radius = 6378137  # meters
    wgs84_flattening = 1 - 1 / 298.257223563
    eartPerim = wgs84_radius * 2 * math.pi
    referenceLat = 40.1912 ##################### Suposed to be Vehicle lattitude

    radLat = math.radians(referenceLat)
    earthRadiusLon = wgs84_radius * math.cos(radLat)/wgs84_flattening
    eartPerimLon = earthRadiusLon * 2 * math.pi

    poseLatLonHei = {}
    poseLatLonHei['lat'] = poseXYZ['y'] * 360 / eartPerim
    poseLatLonHei['lon'] = poseXYZ['x'] * 360 / eartPerimLon
    poseLatLonHei['hei'] = poseXYZ['z']

    return poseLatLonHei

def body2NED(CMDVel, Pose3D):


    #q1 = [0, CMDVel.linearX, CMDVel.linearY, CMDVel.linearZ]
    #q2 = [Pose3D.q0, Pose3D.q1, Pose3D.q2, Pose3D.q3]

    #q1 = qNormal(q1)
    #q2 = qNormal(q2)

    ##rotation = q2*q1*q2'

    #q2inverse = qInverse(q2)
    #qtempotal = qMultiply(q1,q2inverse)
    #q = qMultiply(q2,qtempotal)

    #rotatedVector = q[1:len(q)] #obtain [q1,q2,q3]

    #return rotatedVector

     q0 = Pose3D.q0
     q1 = Pose3D.q1
     q2 = Pose3D.q2
     q3 = Pose3D.q3

     # obtain eulers from quaternion TO BE IMPROVED!!!!!!!!!!!

     #roll = 1/ math.tan((2*(q1*q2+q0*q3))/(q3*q3+q2*q2-q1*q1-q0*q0))
     #pitch = 1/math.sin(-2*(q0*q2-q1*q3))
     #yaw = 1/ math.tan((2*(q0*q1+q3*q2))/(q3*q3-q2*q2-q1*q1+q0*q0))

     # Body velocity (x,y,z)

     bvx = CMDVel.linearX
     bvy = CMDVel.linearY
     bvz = CMDVel.linearZ

     NEDvel = [0,0,0] #[x,y,z]

     #NEDvel[0] = bvx * math.cos(pitch)*math.cos(yaw)   + bvy * (math.sin(roll)*math.sin(pitch)*math.cos(yaw) - math.cos(roll)*math.sin(yaw))   + bvz * (math.cos(roll)*math.sin(pitch)*math.cos(yaw) + math.sin(roll)*math.sin(yaw))
     #NEDvel[1] = bvx * math.cos(pitch)*math.sin(yaw)   + bvy * (math.sin(roll)*math.sin(pitch)*math.sin(yaw) + math.cos(roll)*math.cos(yaw))   + bvz * (math.cos(roll)*math.sin(pitch)*math.sin(yaw) - math.sin(roll)*math.cos(yaw))
     #NEDvel[2] = -bvx * math.sin(pitch)                + bvy * (math.sin(roll)*math.cos(pitch))                                                + bvz * (math.cos(roll)*math.cos(pitch))

     NEDvel[0]=bvx
     NEDvel[1]=bvy
     NEDvel[2]=bvz

     return NEDvel

def qMultiply (q1,q2):

    q1 = qNormal(q1)
    q2 = qNormal(q2)

    # quaternion1
    w1 = q1[0]
    x1 = q1[1]
    y1 = q1[2]
    z1 = q1[3]

    #quaternion2
    w2 = q2[0]
    x2 = q2[1]
    y2 = q2[2]
    z2 = q2[3]

    w = w1*w2 - x1*x2 - y1*y2 - z1*z2
    x = w1*x2 + x1*w2 + y1*z2 - z1*y2
    y = w1*y2 + y1*w2 + z1*x2 - x1*z2
    z = w1*z2 + z1*w2 + x1*y2 - y1*x2

    q = [w,x,y,z]

    q = qNormal(q)
    return q

def qNormal(q1):

    qmodule = math.sqrt(q1[0]*q1[0] + q1[1]*q1[1] + q1[2]*q1[2] + q1[3]*q1[3])
    q = [0,0,0,0]

    if (qmodule == 0):
        qmodule = 0.000000000001

    q[0] = q1[0] / qmodule
    q[1] = q1[1] / qmodule
    q[2] = q1[2] / qmodule
    q[3] = q1[3] / qmodule

    return q

def qConjugate(q1):

    q1 = qNormal(q1)
    q = [0,0,0,0]
    q[0] = q1[0]
    q[1] = -q1[1]
    q[2] = -q1[2]
    q[3] = -q1[3]

    q = qNormal(q)
    return q

def qInverse(q1):

    q1 = qNormal(q1)
    qconjugate = qConjugate(q1)
    qmodule = math.sqrt(q1[0] * q1[0] + q1[1] * q1[1] + q1[2] * q1[2] + q1[3] * q1[3])

    if (qmodule == 0):
        qmodule = 0.000000000001

    q = [0,0,0,0]
    q[0] = qconjugate[0] / qmodule
    q[1] = qconjugate[1] / qmodule
    q[2] = qconjugate[2] / qmodule
    q[3] = qconjugate[3] / qmodule

    q = qNormal(q)
    return q

#####################################################################

if __name__ == '__main__':
    from optparse import OptionParser
    parser = OptionParser("mavproxy.py [options]")

    parser.add_option("--master", dest="master", action='append',
                      metavar="DEVICE[,BAUD]", help="MAVLink master port and optional baud rate",
                      default=[])
    parser.add_option("--udp", dest="udp", action='append', help="run udp server")
    parser.add_option("--tcp", dest="tcp", action='append', help="run tcp server")
    parser.add_option("--out", dest="output", action='append',
                      metavar="DEVICE[,BAUD]", help="MAVLink output port and optional baud rate",
                      default=[])
    parser.add_option("--baudrate", dest="baudrate", type='int',
                      help="default serial baud rate", default=57600)
    parser.add_option("--sitl", dest="sitl",  default=None, help="SITL output port")
    parser.add_option("--streamrate",dest="streamrate", default=4, type='int',
                      help="MAVLink stream rate")
    parser.add_option("--source-system", dest='SOURCE_SYSTEM', type='int',
                      default=255, help='MAVLink source system for this GCS')
    parser.add_option("--source-component", dest='SOURCE_COMPONENT', type='int',
                      default=0, help='MAVLink source component for this GCS')
    parser.add_option("--target-system", dest='TARGET_SYSTEM', type='int',
                      default=0, help='MAVLink target master system')
    parser.add_option("--target-component", dest='TARGET_COMPONENT', type='int',
                      default=0, help='MAVLink target master component')
    parser.add_option("--logfile", dest="logfile", help="MAVLink master logfile",
                      default='mav.tlog')
    parser.add_option("-a", "--append-log", dest="append_log", help="Append to log files",
                      action='store_true', default=False)
    parser.add_option("--quadcopter", dest="quadcopter", help="use quadcopter controls",
                      action='store_true', default=False)
    parser.add_option("--setup", dest="setup", help="start in setup mode",
                      action='store_true', default=False)
    parser.add_option("--nodtr", dest="nodtr", help="disable DTR drop on close",
                      action='store_true', default=False)
    parser.add_option("--show-errors", dest="show_errors", help="show MAVLink error packets",
                      action='store_true', default=False)
    parser.add_option("--speech", dest="speech", help="use text to speach",
                      action='store_true', default=False)
    parser.add_option("--aircraft", dest="aircraft", help="aircraft name", default=None)
    parser.add_option("--cmd", dest="cmd", help="initial commands", default=None, action='append')
    parser.add_option("--console", action='store_true', help="use GUI console")
    parser.add_option("--map", action='store_true', help="load map module")
    parser.add_option(
        '--load-module',
        action='append',
        default=[],
        help='Load the specified module. Can be used multiple times, or with a comma separated list')
    parser.add_option("--mav09", action='store_true', default=False, help="Use MAVLink protocol 0.9")
    parser.add_option("--auto-protocol", action='store_true', default=False, help="Auto detect MAVLink protocol version")
    parser.add_option("--nowait", action='store_true', default=False, help="don't wait for HEARTBEAT on startup")
    parser.add_option("-c", "--continue", dest='continue_mode', action='store_true', default=False, help="continue logs")
    parser.add_option("--dialect",  default="ardupilotmega", help="MAVLink dialect")
    parser.add_option("--rtscts",  action='store_true', help="enable hardware RTS/CTS flow control")
    parser.add_option("--moddebug",  type=int, help="module debug level", default=0)
    parser.add_option("--mission", dest="mission", help="mission name", default=None)
    parser.add_option("--daemon", action='store_true', help="run in daemon mode, do not start interactive shell")
    parser.add_option("--profile", action='store_true', help="run the Yappi python profiler")
    parser.add_option("--state-basedir", default=None, help="base directory for logs and aircraft directories")
    parser.add_option("--version", action='store_true', help="version information")
    parser.add_option("--default-modules", default="log,wp,rally,fence,param,relay,tuneopt,arm,mode,calibration,rc,auxopt,misc,cmdlong,battery,terrain,output", help='default module list')

    (opts, args) = parser.parse_args()

    # warn people about ModemManager which interferes badly with APM and Pixhawk
    if os.path.exists("/usr/sbin/ModemManager"):
        print("WARNING: You should uninstall ModemManager as it conflicts with APM and Pixhawk")

    if opts.mav09:
        os.environ['MAVLINK09'] = '1'
    from pymavlink import mavutil, mavparm
    mavutil.set_dialect(opts.dialect)

    #version information
    if opts.version:
        import pkg_resources
        version = pkg_resources.require("mavproxy")[0].version
        print("MAVProxy is a modular ground station using the mavlink protocol")
        print("MAVProxy Version: " + version)
        sys.exit(1)

    # global mavproxy state
    mpstate = MPState()
    mpstate.status.exit = False
    mpstate.command_map = command_map
    mpstate.continue_mode = opts.continue_mode
    # queues for logging
    mpstate.logqueue = Queue.Queue()
    mpstate.logqueue_raw = Queue.Queue()

    if opts.udp:
        mpstate.udp.connect(opts.udp[0].split(":")[0], int(opts.udp[0].split(":")[1]))
        print("Connected (UDP) to " + mpstate.udp.address + ":" + str(mpstate.udp.port))

    if opts.tcp:
        mpstate.tcp.connect(opts.tcp[0].split(":")[0], int(opts.tcp[0].split(":")[1]))
        print("Client (TCP) connected at " + mpstate.tcp.client[0] + ":" + str(mpstate.tcp.port))

    if opts.speech:
        # start the speech-dispatcher early, so it doesn't inherit any ports from
        # modules/mavutil
        load_module('speech')

    if not opts.master:
        serial_list = mavutil.auto_detect_serial(preferred_list=['*FTDI*',"*Arduino_Mega_2560*", "*3D_Robotics*", "*USB_to_UART*", '*PX4*', '*FMU*'])
        print('Auto-detected serial ports are:')
        for port in serial_list:
              print("%s" % port)

    # container for status information
    mpstate.settings.target_system = opts.TARGET_SYSTEM
    mpstate.settings.target_component = opts.TARGET_COMPONENT

    mpstate.mav_master = []

    mpstate.rl = rline.rline("MAV> ", mpstate)

    def quit_handler(signum = None, frame = None):
        #print 'Signal handler called with signal', signum
        if mpstate.status.exit:
            print('Clean shutdown impossible, forcing an exit')
            sys.exit(0)
        else:
            mpstate.status.exit = True

    # Listen for kill signals to cleanly shutdown modules
    fatalsignals = [signal.SIGTERM]
    try:
        fatalsignals.append(signal.SIGHUP)
        fatalsignals.append(signal.SIGQUIT)
    except Exception:
        pass
    if opts.daemon: # SIGINT breaks readline parsing - if we are interactive, just let things die
        fatalsignals.append(signal.SIGINT)

    for sig in fatalsignals:
        signal.signal(sig, quit_handler)

    load_module('link', quiet=True)

    mpstate.settings.source_system = opts.SOURCE_SYSTEM
    mpstate.settings.source_component = opts.SOURCE_COMPONENT

    # open master link
    for mdev in opts.master:
        if not mpstate.module('link').link_add(mdev):
            sys.exit(1)

    if not opts.master and len(serial_list) == 1:
          print("Connecting to %s" % serial_list[0])
          mpstate.module('link').link_add(serial_list[0].device)
    elif not opts.master:
          wifi_device = '0.0.0.0:14550'
          mpstate.module('link').link_add(wifi_device)


    # open any mavlink output ports
    for port in opts.output:
        mpstate.mav_outputs.append(mavutil.mavlink_connection(port, baud=int(opts.baudrate), input=False))

    if opts.sitl:
        mpstate.sitl_output = mavutil.mavudp(opts.sitl, input=False)

    mpstate.settings.streamrate = opts.streamrate
    mpstate.settings.streamrate2 = opts.streamrate

    if opts.state_basedir is not None:
        mpstate.settings.state_basedir = opts.state_basedir

    msg_period = mavutil.periodic_event(1.0/15)
    heartbeat_period = mavutil.periodic_event(1)
    heartbeat_check_period = mavutil.periodic_event(0.33)

    mpstate.input_queue = Queue.Queue()
    mpstate.input_count = 0
    mpstate.empty_input_count = 0
    if opts.setup:
        mpstate.rl.set_prompt("")

    # call this early so that logdir is setup based on --aircraft
    (mpstate.status.logdir, logpath_telem, logpath_telem_raw) = log_paths()

    if not opts.setup:
        # some core functionality is in modules
        standard_modules = opts.default_modules.split(',')
        for m in standard_modules:
            load_module(m, quiet=True)

    if opts.console:
        process_stdin('module load console')

    if opts.map:
        process_stdin('module load map')

    for module in opts.load_module:
        modlist = module.split(',')
        for mod in modlist:
            process_stdin('module load %s' % mod)

    if 'HOME' in os.environ and not opts.setup:
        start_script = os.path.join(os.environ['HOME'], ".mavinit.scr")
        if os.path.exists(start_script):
            run_script(start_script)
    if 'LOCALAPPDATA' in os.environ and not opts.setup:
        start_script = os.path.join(os.environ['LOCALAPPDATA'], "MAVProxy", "mavinit.scr")
        if os.path.exists(start_script):
            run_script(start_script)

    if opts.aircraft is not None:
        start_script = os.path.join(opts.aircraft, "mavinit.scr")
        if os.path.exists(start_script):
            run_script(start_script)
        else:
            print("no script %s" % start_script)

    if opts.cmd is not None:
        for cstr in opts.cmd:
            cmds = cstr.split(';')
            for c in cmds:
                process_stdin(c)

    if opts.profile:
        import yappi    # We do the import here so that we won't barf if run normally and yappi not available
        yappi.start()

    # log all packets from the master, for later replay
    open_telemetry_logs(logpath_telem, logpath_telem_raw)

    ########################## Jorge Cano CODE ##########################

    PH_Pose3D = Pose3DI(0,0,0,0,0,0,0,0) #1 to avoid indeterminations
    PH_CMDVel = CMDVelI(0,0,0,0,0,0) #1 to avoid indeterminations
    PH_Extra = ExtraI()
    WP_Pose3D = Pose3DI(0,0,0,0,0,0,0,0)

    #####################################################################
    global operation_takeoff
    global on_air
    global time_init_operation_takeoff
    global time_end_operation_takeoff

    operation_takeoff = False
    on_air = False
    time_init_operation_takeoff = 10000000000000000000
    time_end_operation_takeoff = 10000000000000000000
    print("Variables a false")
    # run main loop as a thread

    mpstate.status.thread = threading.Thread(target=main_loop, name='main_loop')
    mpstate.status.thread.daemon = True
    mpstate.status.thread.start()

    ########################## Jorge Cano CODE ##########################

    cfg = config.load(sys.argv[1])
    #Open an ICE TX communication and leave it open in a parallel threat

    PoseTheading = threading.Thread(target=openPose3DChannel, args=(PH_Pose3D,cfg,), name='Pose_Theading')
    PoseTheading.daemon = True
    PoseTheading.start()

    # Open an ICE RX communication and leave it open in a parallel threat

    CMDVelTheading = threading.Thread(target=openCMDVelChannel, args=(PH_CMDVel,cfg,), name='CMDVel_Theading')
    CMDVelTheading.daemon = True
    CMDVelTheading.start()

    # Open an ICE TX communication and leave it open in a parallel threat

    CMDVelTheading = threading.Thread(target=openExtraChannel, args=(PH_Extra,cfg,), name='Extra_Theading')
    CMDVelTheading.daemon = True
    CMDVelTheading.start()

    # Open an ICE channel empty

    CMDVelTheading = threading.Thread(target=openNavdataChannel, args=(cfg,), name='Navdata_Theading')
    CMDVelTheading.daemon = True
    CMDVelTheading.start()

    # # Open an MAVLink TX communication and leave it open in a parallel threat
    #
    PoseTheading = threading.Thread(target=sendCMDVel2Vehicle, args=(PH_CMDVel,PH_Pose3D,), name='TxCMDVel_Theading')
    PoseTheading.daemon = True
    PoseTheading.start()


    # Open an ICE TX communication and leave it open in a parallel threat

    #PoseTheading = threading.Thread(target=openPose3DChannelWP, args=(WP_Pose3D,cfg,), name='WayPoint_Theading')
    #PoseTheading.daemon = True
    #PoseTheading.start()

    ## Open an MAVLink TX communication and leave it open in a parallel threat

    #PoseTheading = threading.Thread(target=sendWayPoint2Vehicle, args=(WP_Pose3D,), name='WayPoint2Vehicle_Theading')
    #PoseTheading.daemon = True
    #PoseTheading.start()

    # Open an MAVLink TX communication and leave it open in a parallel threat

    PoseTheading = threading.Thread(target=landDecision, args=(PH_Extra,), name='LandDecision2Vehicle_Theading')
    PoseTheading.daemon = True
    PoseTheading.start()



    #while True:
    #    time.sleep(1)
    #    Posejarl = PH_Pose3D.getPose3DData()
    #    print (Posejarl)

    #####################################################################

    # use main program for input. This ensures the terminal cleans
    # up on exit
    while (mpstate.status.exit != True):
        try:
            if opts.daemon:
                time.sleep(0.1)
            else:
                input_loop()
        except KeyboardInterrupt:
            if mpstate.settings.requireexit:
                print("Interrupt caught.  Use 'exit' to quit MAVProxy.")

                #Just lost the map and console, get them back:
                for (m,pm) in mpstate.modules:
                    if m.name in ["map", "console"]:
                        if hasattr(m, 'unload'):
                            try:
                                m.unload()
                            except Exception:
                                pass
                        reload(m)
                        m.init(mpstate)

            else:
                mpstate.status.exit = True
                sys.exit(1)

    if opts.profile:
        yappi.get_func_stats().print_all()
        yappi.get_thread_stats().print_all()

    #this loop executes after leaving the above loop and is for cleanup on exit
    for (m,pm) in mpstate.modules:
        if hasattr(m, 'unload'):
            print("Unloading module %s" % m.name)
            m.unload()

    sys.exit(1)
