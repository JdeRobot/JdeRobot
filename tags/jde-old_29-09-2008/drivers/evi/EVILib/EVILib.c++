// EVILib.c++
//
// Access functions to a SONY EVI camera
//
// RS-232C Serial Port Communication by Max Lungarella.
// Email: max.lungarella@aist.go.jp
//
// Copyright (c) 2003 Pic Mickael
//
//----------------------------------------------------------------------------
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//----------------------------------------------------------------------------
//
// Author: Pic Mickael, AIST Japan, 2002.
// Email: mickael.pic@aist.go.jp

#include "EVILib.h"

/*
 * Constructor
 */
EVILib::EVILib()
{
  _port = 1;

  buffer = NULL;
  buffer2 = NULL;
  buffer3 = NULL;
  PanTiltStatus = NULL;

  power = EVILIB_OFF;
  End = EVILIB_ON;

  minGain = 1;
  maxGain = 1;
  minpan = 1;
  maxpan = 1;
  maxPan = 1;
  min_pspeed = 1;
  max_pspeed = 1;
  mintilt = 1;
  maxtilt = 1;
  maxTilt = 1;
  min_tspeed = 1;
  max_tspeed = 1;
  minzoom = 1;
  maxzoom = 1;
  maxZoom = 1;
  min_zspeed = 1;
  max_zspeed = 1;
  min_iris = 1;
  max_iris = 1;
  highLowSignal = 0;
}

/*
 *
 */
EVILib::~EVILib()
{
  End = EVILIB_OFF;

  pthread_mutex_destroy(&AccessBuffer3);
  pthread_mutex_destroy(&TakeAck);
  pthread_mutex_destroy(&TakeInfo);
  pthread_mutex_destroy(&TakeComp);
  pthread_mutex_destroy(&TakeReturn);
  pthread_mutex_destroy(&LaunchThread);
  pthread_mutex_destroy(&BufferDispo);
  pthread_mutex_destroy(&CommandBuffer);
  pthread_cond_destroy(&CommandBufferCond);

  if(buffer != NULL)
    {
      delete[] buffer;
      buffer = NULL;
    }
  if(buffer2 != NULL)
    {
      delete[] buffer2;
      buffer2 = NULL;
    }
  if(buffer3 != NULL)
    {
      delete[] buffer3;
      buffer3 = NULL;
    }
  if(PanTiltStatus != NULL)
    {
      delete[] PanTiltStatus;
      PanTiltStatus = NULL;
    }
}

/*
 * Initialize the data
 * Return 1 on success
 * Return 0 on error
 */
int EVILib::HiddenInit()
{
  buffer = new char[50];
  if(buffer == NULL)
    {
      cerr<<"Error EVILib Init(): buffer\n";
      return 0;
    }
  buffer2 = new char[50];
  if(buffer2 == NULL)
    {
      cerr<<"Error EVILib Init(): buffer2\n";
      return 0;
    }
  buffer3 = new char[50];
  if(buffer3 == NULL)
    {
      cerr<<"Error EVILib Init(): buffer3\n";
      return 0;
    }
  PanTiltStatus = new int[18];
  if(PanTiltStatus == NULL)
    {
      cerr<<"Error EVILib Init(): PanTiltStatus\n";
      return 0;
    }

  _port = -1;

  Nb_Command_Buffer = 0;

  for(int c = 0; c < 18; c ++)
    PanTiltStatus[c] = 0;

  pthread_mutex_init(&AccessBuffer3, NULL);
  pthread_mutex_init(&TakeAck, NULL);
  pthread_mutex_init(&TakeInfo, NULL);
  pthread_mutex_init(&TakeComp, NULL);
  pthread_mutex_init(&TakeReturn, NULL);
  pthread_mutex_init(&LaunchThread, NULL);
  pthread_mutex_init(&BufferDispo, NULL);
  pthread_mutex_init(&CommandBuffer, NULL);

  pthread_cond_init(&CommandBufferCond, NULL);


  pthread_mutex_lock(&TakeAck);
  pthread_mutex_lock(&TakeInfo);
  pthread_mutex_lock(&TakeComp);
  pthread_mutex_lock(&TakeReturn);
  pthread_mutex_lock(&LaunchThread);

  power = EVILIB_OFF;

  waitComp = EVILIB_NO_WAIT_COMP;

  End = EVILIB_ON;

  return 1;
}

/*
 * Fonction handled by a thread.
 * Receive one answer from the camera.
 * Return the number of bytes recevied on success, 0 on error
 */
void *Receiver(void *arg)
{
  EVILib *commonData;
  commonData = (EVILib*)arg;

  int cam_number = 0;
  int sock_number = 0;
  int sub_mes = 0;
  int i;
  unsigned char c;
  int no_signal_reicv = 0;
  int mutex_return = 0;

#if 0
  int nn = 0;
#endif

  pthread_mutex_unlock(&commonData->LaunchThread);
  do
    {

      c = '\0';
      no_signal_reicv = 0;
      i = 0;

      if(commonData->_port < 1)
        {
	  cerr<<"Error EVILib Receiver(void *): There were problems while receiving bytes from the serial port!\n";
	  commonData->answer = 0;
        }

// Read until we get the terminating byte, \xff.
      while((char)c != '\xff' && commonData->End != EVILIB_OFF)
        {
	  switch(read(commonData->_port, &c, 1))
            {
            case -1:
	      commonData->answer = 0;
	      break;
            case 0:
	      no_signal_reicv ++;
	      if(no_signal_reicv >= 500)
                {
		  mutex_return = pthread_mutex_trylock(&commonData->TakeComp);
		  if(mutex_return == 0)
		    pthread_mutex_unlock(&commonData->TakeComp);
		  else
                    if((mutex_return == EBUSY && commonData->waitComp == EVILIB_WAIT_COMP))
		      {
			commonData->answer = 0;
                        commonData->waitComp = EVILIB_NO_WAIT_COMP;
                        pthread_mutex_unlock(&commonData->TakeComp);
                        goto skip;
		      }
		  no_signal_reicv = 0;
                }
	      break;
            case 1:
	      no_signal_reicv = 0;
	      commonData->buffer2[i++] = c;
#if 0
	      printf("%02X",c);
#endif
	      break;
            default: // Either we have an error, read nothing, or read 1 byte...
	      break;
            }
	  usleep(100);
        }
#if 0
      cout<<"\n";
#endif
    skip:


// *** Empties in/output buffer: Warning this may lead to a loss of information!!! ***
#if 0
      if(nn > 16384)
        {
	  tcflush(commonData->_port, TCIFLUSH);
	  nn=0;
        }
      nn += i;
#endif

      cam_number = int(((unsigned char) commonData->buffer2[0] & (unsigned char)'\xF0') >> 4) - 8;
      sock_number = int(((unsigned char) commonData->buffer2[1] & (unsigned char)'\x0F'));

// Check for error message
      switch(int(((unsigned char) commonData->buffer2[1] & (unsigned char)'\xF0') >> 4))
        {
        case 6:  // Error message
	  sub_mes = int(((unsigned char) commonData->buffer2[2] & (unsigned char)'\xF0') >> 4) * 10 + int(((unsigned char) commonData->buffer2[2] & (unsigned char)'\x0F'));
	  switch(sub_mes)
            {
            case 1:
	      cerr<<"Error EVILib Receiver(void *): Message length (>14 bytes) ";
	      break;
            case 2:
	      cerr<<"Error EVILib Receiver(void *): syntax error ";
	      break;
            case 3:
	      commonData->Mutex();
	      cerr<<"Error EVILib Receiver(void *): command buffer full ";
	      break;
            case 4:
	      cerr<<"Error EVILib Receiver(void *): command cancel ";
	      break;
            case 5:
	      cerr<<"Error EVILib Receiver(void *): no sockets (to be cancelled) ";
	      break;
            case 41:
	      cerr<<"Error EVILib Receiver(void *): command not executable ";
	      break;
            default:
	      cerr<<"Error EVILib Receiver(void *): unknown error type ";
	      break;
            }
	  commonData->answer = 0;
	  cerr<<"from camera "<<cam_number<<" socket "<<sock_number<<endl;

// We have to free the mutex.
// Otherwise the server is waiting for an answer that will never come...
	  pthread_mutex_unlock(&commonData->TakeAck);
	  pthread_mutex_unlock(&commonData->TakeInfo);
	  pthread_mutex_unlock(&commonData->TakeReturn);
	  pthread_mutex_unlock(&commonData->TakeComp);
	  commonData->waitComp = EVILIB_NO_WAIT_COMP;
	  pthread_mutex_lock(&commonData->CommandBuffer);
	  if(commonData->Nb_Command_Buffer > 0)
	    {
	      commonData->Nb_Command_Buffer --;
	      pthread_cond_broadcast(&commonData->CommandBufferCond);
	    }
	  pthread_mutex_unlock(&commonData->CommandBuffer);
	  break;

        case 3: // Sent from the peripheral device
#if 0
	  cout<<"Broadcast\n";
#endif
	  switch(sock_number) // In this case sock_number do not contain the socket number...
            {
            case 0: // Address
#if 0
	      cout<<"  Address Set\n";
#endif
	      commonData->answer = i;
	      mutex_return = pthread_mutex_trylock(&commonData->TakeComp);
	      if(mutex_return == 0)
		pthread_mutex_unlock(&commonData->TakeComp);
	      else
                if(mutex_return == EBUSY && commonData->waitComp == EVILIB_WAIT_COMP)
		  {
                    commonData->waitComp = EVILIB_NO_WAIT_COMP;
                    pthread_mutex_unlock(&commonData->TakeComp);
		  }
	      pthread_mutex_lock(&commonData->CommandBuffer);
	      if(commonData->Nb_Command_Buffer > 0)
		{
		  commonData->Nb_Command_Buffer --;
		  pthread_cond_broadcast(&commonData->CommandBufferCond);
		}
	      pthread_mutex_unlock(&commonData->CommandBuffer);
	      break;
            case 8: // Network Change
	      cerr<<"EVILib Receiver(void *): Network Change for camera "<<cam_number<<endl;
	      break;
            default:
	      cerr<<"Error EVILib Receiver(void *): unknown Broadcast type\n";
            }
	  break;
        case 4: // Acknowledgement
#if 0
	  cout<<"Ack\n";
#endif
	  commonData->answer = i;
	  mutex_return = pthread_mutex_trylock(&commonData->TakeAck);
	  if(mutex_return == 0 || mutex_return == EBUSY)
	    pthread_mutex_unlock(&commonData->TakeAck);
// There is no 'semaphore_up(&commandData->CommandBuffer)' here, because the Ack do
//  not empty one of the command buffer.
	  break;

        case 5: // Completion (commands or inquiries)
	  commonData->answer = i;
	  if(i == 3) // Command completion is size 3 only
            {
#if 0
	      cout<<"Command Completion\n";
#endif
	      mutex_return = pthread_mutex_trylock(&commonData->TakeComp);
	      if(mutex_return == 0)
		pthread_mutex_unlock(&commonData->TakeComp);
	      else
                if(mutex_return == EBUSY && commonData->waitComp == EVILIB_WAIT_COMP)
		  {
                    commonData->waitComp = EVILIB_NO_WAIT_COMP;
                    pthread_mutex_unlock(&commonData->TakeComp);
		  }
            }
	  else
            {
#if 0
	      cout<<"Information return\n";
#endif
	      pthread_mutex_lock(&commonData->AccessBuffer3);
	      memcpy(commonData->buffer3, commonData->buffer2, i);
	      pthread_mutex_unlock(&commonData->AccessBuffer3);
	      mutex_return = pthread_mutex_trylock(&commonData->TakeInfo);
	      if(mutex_return == 0 || mutex_return == EBUSY)
		pthread_mutex_unlock(&commonData->TakeInfo);
            }
	  pthread_mutex_lock(&commonData->CommandBuffer);
	  if(commonData->Nb_Command_Buffer > 0)
	    {
	      commonData->Nb_Command_Buffer --;
	      pthread_cond_broadcast(&commonData->CommandBufferCond);
	    }
	  pthread_mutex_unlock(&commonData->CommandBuffer);
	  break;

        case 0:
	  switch(sock_number) // In this case sock_number do not contain the socket number...
            {
            case 1: // Return from IF_Clear (broadcast)
#if 0
	      cout<<"  IF_Clear\n";
#endif
	      mutex_return = pthread_mutex_trylock(&commonData->TakeComp);
	      if(mutex_return == 0)
		{
		pthread_mutex_unlock(&commonData->TakeComp);
		}
	      else
                if(mutex_return == EBUSY && commonData->waitComp == EVILIB_WAIT_COMP)
                {
		  commonData->waitComp = EVILIB_NO_WAIT_COMP;
		  pthread_mutex_unlock(&commonData->TakeComp);
                }
	      pthread_mutex_lock(&commonData->CommandBuffer);
	      if(commonData->Nb_Command_Buffer > 0)
		{
		  commonData->Nb_Command_Buffer --;
		  pthread_cond_broadcast(&commonData->CommandBufferCond);
		}
	      pthread_mutex_unlock(&commonData->CommandBuffer);
	      break;
            case 7: // IR Receiver Return or signal output change
	      c = commonData->buffer2[2];
	      if(c == '\x04') // signal output change
		{
		  c = commonData->buffer2[4];
		  if(c == '\x01') // Low -> High edge
		    {
#if 0
		      cout<<"Low -> High edge\n";
#endif
		      commonData->highLowSignal = 1;
		    }
		  if(c == '\x00') // High -> Low edge
		    {
#if 0
		      cout<<"High -> Low edge\n";
#endif
		      commonData->highLowSignal = -1;
		    }
		  break;

// Is there any mutex to free ?

		}
	      if(c == '\x7D') // IR receive return
		{
#if 0
		  cout<<"Return for IR Commander\n";
#endif
		  commonData->answer = i;
		  pthread_mutex_lock(&commonData->AccessBuffer3);
		  memcpy(commonData->buffer3, commonData->buffer2, i);
		  pthread_mutex_unlock(&commonData->AccessBuffer3);
		  mutex_return = pthread_mutex_trylock(&commonData->TakeReturn);
		  if(mutex_return == 0 || mutex_return == EBUSY)
		    pthread_mutex_unlock(&commonData->TakeReturn);
		  pthread_mutex_lock(&commonData->CommandBuffer);
		  if(commonData->Nb_Command_Buffer > 0)
		    {
		      commonData->Nb_Command_Buffer --;
		      pthread_cond_broadcast(&commonData->CommandBufferCond);
		    }
		  pthread_mutex_unlock(&commonData->CommandBuffer);
		  break;
		}
            default:
	      cerr<<"Error EVILib Receiver(void *): unknown return type\n";
	      break;
            }
	  break;
        default:
	  cerr<<"Error EVILib Receiver(void *): Unknown message received from camera\n";
	  break;
        }
    }
  while(commonData->End != EVILIB_OFF);

  return NULL;
}

/*
 * Send a command to the camera
 * Return the number of bytes sended
 */
int EVILib::SendCommand(unsigned char *data, int len)
{
  int num;
// We do not allow more than two commands at the same time in the camera
  pthread_mutex_lock(&CommandBuffer); 
#if 0
  printf("  %s\n", buffer);
#endif
  do
    {
      if(Nb_Command_Buffer > 1)
	pthread_cond_wait(&CommandBufferCond, &CommandBuffer);
    }
  while(Nb_Command_Buffer > 1);
  num = asciiToPackedHex((unsigned char *)data ,len);
  if(write(_port, data, num) != num)
    {
      cerr<<"Error EVILib SendCommand(unsigned char *, int)\n";
      pthread_mutex_unlock(&CommandBuffer);
      return 0;
    }
  Nb_Command_Buffer ++;
  pthread_mutex_unlock(&CommandBuffer);
// *** Empties in/output buffer: Warning this may lead to a loss of information!!! ***
#if 0
  tcflush(_port, TCOFLUSH);
#endif
  return num;
}

/*
 * Open Serial Port - terminal settings
 * Return 1 on success, 0 on error
 */
int EVILib::Open(int id, char *portname)
{
  struct termios newtio;

  if(_port >= 1) // Port already open
    {
      cerr<<"Error EVILib Open(int, char *) Port already open\n";
      return 0;
    }

//    _port = open(portname, O_RDWR | O_NOCTTY | O_NONBLOCK | O_NDELAY);
  _port = open(portname, O_CREAT | O_RDWR| O_NOCTTY );
  fcntl(_port, F_SETFL, 0);

  if(_port == -1)
    {
      cerr<<"Errro EVILib Open(int, char *) ";
      cerr<<errno<<" ";
      switch(errno)
        {
        case EEXIST:
	  cerr<<"EEXIST\n";
	  break;
        case EISDIR:
	  cerr<<"EISDIR\n";
	  break;
        case EACCES:
	  cerr<<"EACCES\n";
	  break;
        case ENAMETOOLONG:
	  cerr<<"ENAMETOOLONG\n";
	  break;
        case ENOENT:
	  cerr<<"ENOENT\n";
	  break;
        case ENOTDIR:
	  cerr<<"ENOTDIR\n";
	  break;
        case ENXIO:
	  cerr<<"ENXIO\n";
	  break;
        case ENODEV:
	  cerr<<"ENODEV\n";
	  break;
        case EROFS:
	  cerr<<"EROFS\n";
	  break;
        case ETXTBSY:
	  cerr<<"ETXTBSY\n";
	  break;
        case EFAULT:
	  cerr<<"EFAULT\n";
	  break;
        case ELOOP:
	  cerr<<"ELOOP\n";
	  break;
        case ENOSPC:
	  cerr<<"ENOSPC\n";
	  break;
        case ENOMEM:
	  cerr<<"ENOMEM\n";
	  break;
        case EMFILE:
	  cerr<<"EMFILE\n";
	  break;
        case ENFILE:
	  cerr<<"ENFILE\n";
	  break;
        case EBADF:
// This can happen if you set a wrong port name
// Usually this happen is you don't have the rights on the device...
	  cerr<<"EBADF\n";
	  break;
        default:
	  cerr<<"UNKNOW\n";
	  break;
        };
      return 0;
    }
  else
    {
      tcgetattr(_port, &_oldtio); 	// Save current port settings
      bzero(&newtio, sizeof(newtio));	// Clear struct for new port settings

      newtio.c_iflag = IGNBRK;
      newtio.c_oflag = 0;
      newtio.c_cflag = B9600 | CS8 | CSIZE | CLOCAL | CREAD;
      newtio.c_lflag = 0;
      newtio.c_cc[VMIN] = newtio.c_cc[VTIME] = 0;

      tcflush(_port, TCIFLUSH);
      if(tcsetattr(_port, TCSANOW, &newtio) < 0)
        {
	  cerr<<"Error EVILib Open(int, char *) tcsetattr for new port settings\n";
	  return 0;
        }

      if(fcntl(_port, F_SETFL, FNDELAY) < 0)
        {
	  cerr<<"Error EVILib Open(int, char *) fcntl\n";
	  return 0;
        }

      pthread_create(&threadReceiver, NULL, Receiver, this);

// We are waiting for the thread to be launch (synchronization...)
      pthread_mutex_lock(&LaunchThread);

// This must be done before starting communication
      if(AddressSet() == 0)
        {
	  cerr<<"Error EVILib Open(int, char *) AddressSet\n";
	  return 0;
        }
      if(IF_Clear() == 0)
        {
	  cerr<<"Error EVILib Open(int, char*) IF_Clear\n";
	  return 0;
        }

// Set the Id of the camera
      Id_cam = id;

// Set the power state of the camera
      power = PowerInq();

      cout<<"Serial setup OK ... \n";
      return 1;
    }
}

/*
 * Restore old port settings and close Serial Port
 * Return 1 on success, 0 on error
 */
int EVILib::Close()
{
  if(_port < 1) // How can I close an already closed serial port?
    {
      cerr<<"Error EVILib Open(int, char *) serial port already closed\n";
      return 0;
    }
  if(close(_port)!=0)
    {
      cerr<<"Error EVILib Open(int, char *) during closing serial port\n";
      return 0;
    }	
  tcsetattr(_port, TCSANOW, &_oldtio);
  _port = -1;

  End = EVILIB_OFF;

  pthread_join(threadReceiver, NULL);

  return 1;
}

/*
 * Display the value of the semaphore
 */
void EVILib::Mutex()
{
  cout<<"AccessBuffer3: ";
  mutex_return = pthread_mutex_trylock(&AccessBuffer3);
  if(mutex_return == EBUSY)
    cout<<"locked\n";
  else
    cout<<"unlocked\n";
  if(mutex_return == 0)
    pthread_mutex_unlock(&AccessBuffer3);
  cout<<"TakeAck: ";
  mutex_return = pthread_mutex_trylock(&TakeAck);
  if(mutex_return == EBUSY)
    cout<<"locked\n";
  else
    cout<<"unlocked\n";
  if(mutex_return == 0)
    pthread_mutex_unlock(&TakeAck);
  cout<<"TakeInfo: ";
  mutex_return = pthread_mutex_trylock(&TakeInfo);
  if(mutex_return == EBUSY)
    cout<<"locked\n";
  else
    cout<<"unlocked\n";
  if(mutex_return == 0)
    pthread_mutex_unlock(&TakeInfo);
  cout<<"TakeComp: ";
  mutex_return = pthread_mutex_trylock(&TakeComp);
  if(mutex_return == EBUSY)
    cout<<"locked\n";
  else
    cout<<"unlocked\n";
  if(mutex_return == 0)
    pthread_mutex_unlock(&TakeComp);
  cout<<"TakeReturn: ";
  mutex_return = pthread_mutex_trylock(&TakeReturn);
  if(mutex_return == EBUSY)
    cout<<"locked\n";
  else
    cout<<"unlocked\n";
  if(mutex_return == 0)
    pthread_mutex_unlock(&TakeReturn);
  cout<<"LaunchThread: ";
  mutex_return = pthread_mutex_trylock(&LaunchThread);
  if(mutex_return == EBUSY)
    cout<<"locked\n";
  else
    cout<<"unlocked\n";
  if(mutex_return == 0)
    pthread_mutex_unlock(&LaunchThread);
  cout<<"BufferDispo: ";
  mutex_return = pthread_mutex_trylock(&BufferDispo);
  if(mutex_return == EBUSY)
    cout<<"locked\n";
  else
    cout<<"unlocked\n";
  if(mutex_return == 0)
    pthread_mutex_unlock(&BufferDispo);
  cout<<"CommandBuffer: ";
  mutex_return = pthread_mutex_trylock(&CommandBuffer);
  if(mutex_return == EBUSY)
    cout<<"locked\n";
  else
    cout<<"unlocked\n";
  if(mutex_return == 0)
    pthread_mutex_unlock(&CommandBuffer);
}

/*
 * Transform the data of Hex to the signal send to the camera
 */
int EVILib::asciiToPackedHex(unsigned char *data, int len)
{
  int i,j;

  for(i = 0,j = 0; i < len; i ++, j ++)
    {
      if(data[i] >= (unsigned char)'0' && data[i] <= (unsigned char)'9')
	data[j] = data[i] - 48;
      else
	data[j] = data[i] - 55;
      data[j] <<= 4;
      i++;
      if (data[i] >= (unsigned char)'0' && data[i] <= (unsigned char)'9')
	data[j] |= data[i] - 48;
      else
	data[j] |= data[i] - 55;
    }
  return j;
}

/*
 * This function creates a string in the form: "0Y0Y0Y0Y"
 *  where YYYY is the hex representation of "pos".
 * This is needed for some messages to the Sony EVI camera.
 */
void EVILib::make0XString(int pos, char *data, int len)
{
  char temp[80];
  int i, j;

  sprintf(data, "%X", pos);
  if(int(strlen(data)) < len)
    {
      j = len - strlen(data);
      for(i = 0; i < j; i ++)
	temp[i] = '0';
      strcpy(&temp[i], data);
    }
  else
    {
      j = strlen(data) - len;
      strcpy(temp, &data[j]);
    }
  for(i = 0, j = 0; temp[i] != '\0'; i ++, j ++)
    {
      data[j] = '0';
      j ++;
      data[j] = temp[i];
    }
  data[j] = '\0';
}

/*
 * Generate a table a bits from 'x'
 */
void EVILib::bitGenerator(unsigned char x, int *table)
{
  int ct;

  for(ct = 0; ct < 8; ct ++)
    table[ct] = 0;

  for (ct = 7; x != 0; x>>=1)
    {
      if(x & 01)
	table[ct] = 1;
      else
	table[ct] = 0;
      ct --;
    }
}

/*
 * Return the value set in the range of the type
 * type is PAN, TILT, ZOOM
 * limit is the maximum degree of the type
 * maxi is the maximum range of the type
 */
int EVILib::convert(int type, float value, float limit, float maxi)
{
  double x;

  switch(type)
    {
    case EVILIB_PAN:
    case EVILIB_TILT:
      x = value / limit * maxi;
      if(x > maxi)
	x = maxi;
      else
        if(x < -maxi)
	  x = -maxi;
      break;
    case EVILIB_ZOOM:
      x = value / limit * maxi; 
      if(x < 0.0)
	x = 0.0;
      else
        if(x > maxi)
	  x = maxi;
      break;
    default:
      return 0;
    }
  return int(rint(x));
}

/*
 * Return the value contain in 'buffer3' from 'start' for 'length' bits
 *  limit and maxi are used to convert the value from Hexa to float
 */
float EVILib::deconvert(float limit, float maxi, int start, int length)
{
  int c, i, y;
  y = 0;
  c = 1;

  for(i = length - 1; i >= 0; i --)
    {
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x0'){y += 0 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x1'){y += 1 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x2'){y += 2 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x3'){y += 3 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x4'){y += 4 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x5'){y += 5 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x6'){y += 6 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x7'){y += 7 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x8'){y += 8 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x9'){y += 9 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\xA'){y += 10 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\xB'){y += 11 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\xC'){y += 12 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\xD'){y += 13 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\xE'){y += 14 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\xF'){y += 15 * c; goto suite;}
    suite:
      c = c * 16;
    }

// if the first digit is not a '0', then we have a negative number
  if(buffer3[start] != '\x0')
    y -= c;

  return (y / maxi * limit);
}

/*
 * Return the value contain in 'buffer3' from 'start' for 'length' bits
 */
float EVILib::deconvert(int start, int length)
{
  int c, i, y;
  y = 0;
  c = 1;

  for(i = length - 1; i >= 0; i --)
    {
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x0'){y += 0 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x1'){y += 1 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x2'){y += 2 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x3'){y += 3 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x4'){y += 4 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x5'){y += 5 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x6'){y += 6 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x7'){y += 7 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x8'){y += 8 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\x9'){y += 9 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\xA'){y += 10 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\xB'){y += 11 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\xC'){y += 12 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\xD'){y += 13 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\xE'){y += 14 * c; goto suite;}
      if((unsigned char) buffer3[start + i] == (unsigned char)'\xF'){y += 15 * c; goto suite;}
    suite:
      c = c * 16;
    }
  return y;
}

//--------------------------------------------------------------------------------------

/*
 * Send AddressSet command and IF_Clear command before starting communication.
 * Return 1 on success, 0 on error
 */
int EVILib::AddressSet()
{
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "883001FF");
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      goto bail;
    }
  pthread_mutex_unlock(&BufferDispo);
  waitComp = EVILIB_WAIT_COMP;
  pthread_mutex_lock(&TakeComp);
// receiv 'ACK'
  return answer;
 bail:
  return 0;
}

/*
 * Send AddressSet command and IF_Clear command before starting communication.
 * Return 1 on success, 0 on error
 */
int EVILib::IF_Clear()
{
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "88010001FF");
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      goto bail;
    }
  pthread_mutex_unlock(&BufferDispo);
  waitComp = EVILIB_WAIT_COMP;
  pthread_mutex_lock(&TakeComp);
// receiv 'ACK'
  return answer;
 bail:
  return 0;
}

/*
 * socket: socket number, 0 or 1
 * Return 1 on success, 0 on error
 */
int EVILib::CommandCancel(int socket)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d2%dFF", Id_cam, socket);
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
      pthread_mutex_lock(&TakeAck);
// receiv 'ACK'
      if(answer < 1)
	return 0;

// We do not take the completion for this command, because there is probably another command waiting
//  for the completion of the command we cancel. When the completion of cancel will arrive, the previous
//  command as to take it to be set free.
      return 1;
    }
  return 0;
}

/*
 * When camera main power is on, camera can be changed to Power Save Mode
 * type: EVILIB_ON  : set power on
 *       EVILIB_Off : set power off
 * Return 1 on success, 0 on error
 */
int EVILib::Power(int type)
{
  float x1, x2, y1, y2, z1, z2;
  timespec *zzz;
  zzz = new timespec;
  zzz->tv_sec = 10;
  zzz->tv_nsec = 0;

  switch(type)
    {
    case EVILIB_ON:
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d01040002FF", Id_cam);
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
      pthread_mutex_lock(&TakeAck);
// receiv 'ACK'

//-----------------------------------------
// It seems that the EVI-D30(31) send the completion before the actual completion
//  of the power on (when the camera is back to home position), that's why there
//  is the check loop for the position after that.
// The other EVI (D100 & D70) will wait for the real completion, and go through the
//  loop only once.
      waitComp = EVILIB_WAIT_COMP;
      pthread_mutex_lock(&TakeComp);
//-----------------------------------------

      if(answer < 1)
	return 0;

      power = EVILIB_ON;

// We put a small sleep because we need time to send to POWER command to the camera
// And then, we wait until the camera have finish the power ON procedure.
// This can be check by the values of the position of the camera that are out of the predefined range
//      nanosleep(zzz, NULL);

      do
        {
	  do
            {
	      if(Pan_TiltPosInq(x1, y1) == 0)
		goto bail;
	      if(ZoomPosInq(z1) == 0)
		goto bail;
//                reliable_usleep(__EVILIB_Waiting_Time__);
	      if(Pan_TiltPosInq(x2, y2) == 0)
		goto bail;
	      if(ZoomPosInq(z2) == 0)
		goto bail;
            }
	  while(x1 != x2 || y1 != y2 || z1 != z2);
        }
      while(x1 < minpan || x2 > maxpan ||
	    y1 < mintilt || y2 > maxtilt ||
	    z1 < minzoom || z2 > maxzoom);

      break;
    case EVILIB_OFF:
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d01040003FF", Id_cam);
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  goto bail;
        }
      pthread_mutex_unlock(&BufferDispo);
      pthread_mutex_lock(&TakeAck);
// receiv 'ACK'
      if(answer < 1)
	goto bail;

      power = EVILIB_OFF;
      break;
    default:
      goto bail;
    }
  return 1;
 bail:
  return 0;
}

/*
 * Iris Setting. Enable on AE_Manual or Iris_Priority
 * type: EVILIB_RESET
 *       EVILIB_UP
 *       EVILIB_DOWN
 *       EVILIB_DIRECT --> need 'cmd'
 * cmd: ExpComp Position min_iris to max_iris
 *       Check camera documentation for values
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVILib::Iris(int type, int cmd, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_RESET:
	  sprintf(buffer, "8%d01040B00FF", Id_cam);
	  break;
        case EVILIB_UP:
	  sprintf(buffer, "8%d01040B02FF", Id_cam);
	  break;
        case EVILIB_DOWN:
	  sprintf(buffer, "8%d01040B03FF", Id_cam);
	  break;
        case EVILIB_DIRECT:
	  if(cmd >= min_iris && cmd <= max_iris)
            {
	      sprintf(buffer, "8%d01044B", Id_cam);
	      make0XString(cmd, &buffer[strlen(buffer)], 4);
	      strcat(buffer, "FF");
            }
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * Gain Setting. Enable on AE_Manual only
 * type: EVILIB_RESET
 *       EVILIB_UP
 *       EVILIB_DOWN
 *       EVILIB_DIRECT --> need 'setting'
 * setting: EVILib_minGain to EVILib_maxGain
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVILib::Gain(int type, int setting, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_RESET:
	  sprintf(buffer, "8%d01040C00FF", Id_cam);
	  break;
        case EVILIB_UP:
	  sprintf(buffer, "8%d01040C02FF", Id_cam);
	  break;
        case EVILIB_DOWN:
	  sprintf(buffer, "8%d01040C03FF", Id_cam);
	  break;
        case EVILIB_DIRECT:
	  if(setting >= minGain && setting <= maxGain)
            {
	      sprintf(buffer, "8%d01044C", Id_cam);
	      make0XString(setting, &buffer[strlen(buffer)], 4);
	      strcat(buffer, "FF");
            }
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * Backlight compensation.
 * Gain-up to 6 dB max.
 * type: EVILIB_ON
 *       EVILIB_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVILib::Backlight(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01043302FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01043303FF", Id_cam);
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * Preset memory for memorize camera condition
 * type: EVILIB_RESET --> need 'position'
 *       EVILIB_SET --> need 'position'
 *       EVILIB_RECALL -- need 'position'
 * position: 0 to 5
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVILib::Memory(int type, int position, int waitC)
{
  if(power == EVILIB_ON)
    {
      if(position >= 0 && position <= 5)
        {
	  pthread_mutex_lock(&BufferDispo);
	  switch(type)
            {
            case EVILIB_RESET:
	      sprintf(buffer, "8%d01043F000%XFF", Id_cam, position);
	      break;
            case EVILIB_SET:
	      sprintf(buffer, "8%d01043F010%XFF", Id_cam, position);
	      break;
            case EVILIB_RECALL:
	      sprintf(buffer, "8%d01043F020%XFF", Id_cam, position);
	      break;
            default:
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
	  pthread_mutex_lock(&TakeAck);
// Wait for Completion
	  if(waitC == EVILIB_WAIT_COMP)
            {
	      waitComp = EVILIB_WAIT_COMP;
	      pthread_mutex_lock(&TakeComp);
            }
	  if(answer < 1)
	    return 0;
	  return 1;
        }
    }
  return 0;
}

/*
 * Enable/Disable for IR remote commander
 * type: EVILIB_ON
 *       EVILIB_OFF
 *       EVILIB_ON_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVILib::IR_Receive(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01060802FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01060803FF", Id_cam);
	  break;
        case EVILIB_ON_OFF:
	  sprintf(buffer, "8%d01060810FF", Id_cam);
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_lock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * Send replies what command received from IR Commander
 * type: EVILIB_ON
 *       EVILIB_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVILib::IR_ReceiveReturn(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d017D01030000FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d017D01130000FF", Id_cam);
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * type: EVILIB_UP --> need 'pan_speed' & 'tilt_speed'
 *       EVILIB_DOWN --> need 'pan_speed' & 'tilt_speed'
 *       EVILIB_LEFT --> need 'pan_speed' & 'tilt_speed'
 *       EVILIB_RIGHT --> need 'pan_speed' & 'tilt_speed'
 *       EVILIB_UPLEFT --> need 'pan_speed' & 'tilt_speed'
 *       EVILIB_UPRIGHT --> need 'pan_speed' & 'tilt_speed'
 *       EVILIB_DOWNLEFT --> need 'pan_speed' & 'tilt_speed'
 *       EVILIB_DOWNRIGHT --> need 'pan_speed' & 'tilt_speed'
 *       EVILIB_STOP --> need 'pan_speed' & 'tilt_speed'
 *       EVILIB_ABSOLUTE --> need 'pan_speed' & 'tilt_speed' & 'pan_pos' & 'tilt_pos'
 *       EVILIB_RELATIVE --> need 'pan_speed' & 'tilt_speed' & 'pan_pos' & 'tilt_pos'
 *       EVILIB_HOME
 *       EVILIB_RESET
 * pan_speed: pan speed EVILIB_min_pspeed to EVILIB_max_pspeed
 * tilt_speed: tilt speed EVILIB_min_tspeed to EVILIB_max_tspeed
 * pan_pos: pan position: approx. -EVILIB_maxpan to +EVILIB_maxpan
 * tilt_pos: tilt position: approx. -EVILIB_mintilt to +EVILIB_maxtilt
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVILib::Pan_TiltDrive(int type, int pan_speed, int tilt_speed, float pan_pos, float tilt_pos, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_HOME:
	  sprintf(buffer, "8%d010604FF", Id_cam);
	  break;
        case EVILIB_RESET:
	  sprintf(buffer, "8%d010605FF", Id_cam);
	  break;
        default:
	  if(pan_speed >= min_pspeed && pan_speed <= max_pspeed &&
	     tilt_speed >= min_tspeed && tilt_speed <= max_tspeed)
            {
	      switch(type)
                {
                case EVILIB_ABSOLUTE:
		  sprintf(buffer, "8%d010602%02X%02X", Id_cam, pan_speed, tilt_speed);
		  make0XString(convert(EVILIB_PAN, pan_pos, maxpan, maxPan), &buffer[strlen(buffer)], 4);
		  make0XString(convert(EVILIB_TILT, tilt_pos, maxtilt, maxTilt), &buffer[strlen(buffer)], 4);
		  strcat(buffer, "FF");
		  break;
                case EVILIB_RELATIVE:
		  sprintf(buffer, "8%d010603%02X%02X", Id_cam, pan_speed, tilt_speed);
		  make0XString(convert(EVILIB_PAN, pan_pos, maxpan, maxPan), &buffer[strlen(buffer)], 4);
		  make0XString(convert(EVILIB_TILT, tilt_pos, maxtilt, maxTilt), &buffer[strlen(buffer)], 4);
		  strcat(buffer, "FF");
		  break;
                default:
		  sprintf(buffer, "8%d010601%02X%02X", Id_cam, pan_speed, tilt_speed);
		  switch(type)
                    {
                    case EVILIB_UP:
		      strcat(buffer, "0301FF");
		      break;
                    case EVILIB_DOWN:
		      strcat(buffer, "0302FF");
		      break;
                    case EVILIB_LEFT:
		      strcat(buffer, "0103FF");
		      break;
                    case EVILIB_RIGHT:
		      strcat(buffer, "0203FF");
		      break;
                    case EVILIB_UPLEFT:
		      strcat(buffer, "0101FF");
		      break;
                    case EVILIB_UPRIGHT:
		      strcat(buffer, "0201FF");
		      break;
                    case EVILIB_DOWNLEFT:
		      strcat(buffer, "0102FF");
		      break;
                    case EVILIB_DOWNRIGHT:
		      strcat(buffer, "0202FF");
		      break;
                    case EVILIB_STOP:
		      strcat(buffer, "0303FF");
		      break;
                    default:
		      pthread_mutex_unlock(&BufferDispo);
		      return 0;
                    }
                }
	      break;
            }
	  pthread_mutex_unlock(&BufferDispo);
     return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1) {
         return 0;

      }
      return 1;
    }
  return 0;
}

/*
 * Pan/Tilt limit set
 * type: EVILIB_SET --> need 'mode' & 'pan_pos' & 'tilt_pos'
 *       EVILIB_CLEAR --> need 'mode' & 'pan_pos' & 'tilt_pos'
 * mode: EVILIB_UPRIGHT
 *       EVILIB_DOWNLEFT
 * pan_pos: pan position: approx. -EVILIB_maxpan to +EVILIB_maxpan
 * tilt_pos: tilt position: approx. -EVILIB_mintilt to +EVILIB_maxtilt
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVILib::Pan_TiltLimitSet(int type, int mode, float pan_pos, float tilt_pos, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_SET:
	  switch(mode)
            {
            case EVILIB_UPRIGHT:
	      sprintf(buffer, "8%d010607000%d", Id_cam, 1);
	      break;
            case EVILIB_DOWNLEFT:
	      sprintf(buffer, "8%d010607000%d", Id_cam, 0);
	      break;
            default:
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  make0XString(convert(EVILIB_PAN, pan_pos, maxpan, maxPan), &buffer[strlen(buffer)], 4);
	  make0XString(convert(EVILIB_TILT, tilt_pos, maxtilt, maxTilt), &buffer[strlen(buffer)], 4);
	  strcat(buffer, "FF");
	  break;
        case EVILIB_CLEAR:
	  switch(mode)
            {
            case EVILIB_UPRIGHT:
	      sprintf(buffer, "8%d010607010%d", Id_cam, 1);
	      break;
            case EVILIB_DOWNLEFT:
	      sprintf(buffer, "8%d010607010%d", Id_cam, 0);
	      break;
            default:
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  make0XString(convert(EVILIB_PAN, pan_pos, maxpan, maxPan), &buffer[strlen(buffer)], 4);
	  make0XString(convert(EVILIB_TILT, tilt_pos, maxtilt, maxTilt), &buffer[strlen(buffer)], 4);
	  strcat(buffer, "FF");
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * Return on success: EVILIB_ON
 *                    EVILIB_OFF
 * Return on error: 0
 */
int EVILib::PowerInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090400FF", Id_cam);
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      return 0;
    }
  pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
  pthread_mutex_lock(&TakeInfo);
  if(answer < 1)
    return 0;
  pthread_mutex_lock(&AccessBuffer3);
  c = buffer3[2];
  pthread_mutex_unlock(&AccessBuffer3);
  if(c == '\x02')
    {
      power = EVILIB_ON;
      return EVILIB_ON;
    }
  if(c == '\x03')
    {
      power = EVILIB_OFF;
      return EVILIB_OFF;
    }
  return 0;
}

/*
 * zoom: contain the zoom position of the camera
 * Return 1 on success, 0 on error
 */
int EVILib::ZoomPosInq(float &zoom)
{
  zoom = -1.0;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d090447FF", Id_cam);
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
      pthread_mutex_lock(&TakeInfo);
      if(answer < 1)
	return 0;
      pthread_mutex_lock(&AccessBuffer3);
      zoom = (deconvert(2, 4) / maxZoom) * maxzoom;
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * Return on success: EVILIB_AUTO
 *                    EVILIB_MANUAL
 * Return on error: 0
 */
int EVILib::FocusModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090438FF", Id_cam);
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      return 0;
    }
  pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
  pthread_mutex_lock(&TakeInfo);
  if(answer < 1)
    return 0;
  pthread_mutex_lock(&AccessBuffer3);
  c = buffer3[2];
  pthread_mutex_unlock(&AccessBuffer3);
  if(c == '\x02')
    return EVILIB_AUTO;
  if(c == '\x03')
    return EVILIB_MANUAL;
  return 0;
}

/*
 * focus: contain the focus position of the camera
 * Return 1 on success, 0 on error
 */
int EVILib::FocusPosInq(int &focus)
{
  focus = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d090448FF", Id_cam);
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
      pthread_mutex_lock(&TakeInfo);
      if(answer < 1)
	return 0;
      pthread_mutex_lock(&AccessBuffer3);
      focus = int(deconvert(2, 4));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}
/*
 * Return on success: EVILIB_AUTO
 *                    EVILIB_INDOOR
 *                    EVILIB_OUTDOOR
 *                    EVILIB_ONEPUSH_MODE
 *                    EVILIB_ATW
 *                    EVILIB_MANUAL
 * Return on error: 0
 */
int EVILib::WBModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090435FF", Id_cam);
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      return 0;
    }
  pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
  pthread_mutex_lock(&TakeInfo);
  if(answer < 1)
    return 0;
  pthread_mutex_lock(&AccessBuffer3);
  c = buffer3[2];
  pthread_mutex_unlock(&AccessBuffer3);
  if(c == '\x00')
    return EVILIB_AUTO;
  if(c == '\x01')
    return EVILIB_INDOOR;
  if(c == '\x02')
    return EVILIB_OUTDOOR;
  if(c == '\x03')
    return EVILIB_ONEPUSH_MODE;
  if(c == '\x04')
    return EVILIB_ATW;
  if(c == '\x05')
    return EVILIB_MANUAL;
  return 0;
}

/*
 * Return on success: EVILIB_AUTO
 *                    EVILIB_MANUAL
 *                    EVILIB_SHUTTER_PRIO
 *                    EVILIB_IRIS_PRIO
 *                    EVILIB_GAIN_PRIO
 *                    EVILIB_BRIGHT
 *                    EVILIB_SHUTTER_AUTO
 *                    EVILIB_IRIS_AUTO
 *                    EVILIB_GAIN_AUTO
 * Return on error: 0
 */
int EVILib::AEModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090439FF", Id_cam);
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      return 0;
    }
  pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
  pthread_mutex_lock(&TakeInfo);
  if(answer < 1)
    return 0;
  pthread_mutex_lock(&AccessBuffer3);
  c = buffer3[2];
  pthread_mutex_unlock(&AccessBuffer3);
  if(c == '\x00')
    return EVILIB_AUTO;
  if(c == '\x03')
    return EVILIB_MANUAL;
  if(c == '\x0A')
    return EVILIB_SHUTTER_PRIO;
  if(c == '\x0B')
    return EVILIB_IRIS_PRIO;
  if(c == '\x0C')
    return EVILIB_GAIN_PRIO;
  if(c == '\x0D')
    return EVILIB_BRIGHT;
  if(c == '\x1A')
    return EVILIB_SHUTTER_AUTO;
  if(c == '\x1B')
    return EVILIB_IRIS_AUTO;
  if(c == '\x1C')
    return EVILIB_GAIN_AUTO;
  return 0;
}

/*
 * iris: contain the iris position of the camera
 * Return 1 on success, 0 on error
 */
int EVILib::IrisPosInq(int &iris)
{
  iris = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d09044BFF", Id_cam);
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
      pthread_mutex_lock(&TakeInfo);
      if(answer < 1)
	return 0;
      pthread_mutex_lock(&AccessBuffer3);
      iris = int(deconvert(2, 4));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * gain: contain the gain of the camera (check to doc for the values)
 * Return 1 on success, 0 on error
 */
int EVILib::GainPosInq(int &gain)
{
  gain = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d09044CFF", Id_cam);
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
      pthread_mutex_lock(&TakeInfo);
      if(answer < 1)
	return 0;
      pthread_mutex_lock(&AccessBuffer3);
      gain = int(deconvert(2, 4));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * Return on success: EVILIB_ON
 *                    EVILIB_OFF
 * Return on error: 0
 */
int EVILib::BacklightModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090433FF", Id_cam);
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      return 0;
    }
  pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
  pthread_mutex_lock(&TakeInfo);
  if(answer < 1)
    return 0;
  pthread_mutex_lock(&AccessBuffer3);
  c = buffer3[2];
  pthread_mutex_unlock(&AccessBuffer3);
  if(c == '\x02')
    return EVILIB_ON;
  if(c == '\x03')
    return EVILIB_OFF;
  return 0;
}

/*
 * memory: contain the preset memory for memorize camera condition
 * Return 1 on success, 0 on error
 */
int EVILib::MemoryInq(int &memory)
{
  memory = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d09043FFF", Id_cam);
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
      pthread_mutex_lock(&TakeInfo);
      if(answer < 1)
	return 0;
      pthread_mutex_lock(&AccessBuffer3);
      memory = int(deconvert(2, 1));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * After the completion of Pan_TiltModeInq, you can check the status with the functions
 *  provided
 * Return 1 on success, 0 on error
 */
int EVILib::Pan_TiltModeInq()
{
  unsigned char alpha;
  unsigned char beta;
  int low[8];
  int high[8];
  int ct;

  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090610FF", Id_cam);
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      return 0;
    }
  pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
  pthread_mutex_lock(&TakeInfo);
  if(answer < 1)
    return 0;
  pthread_mutex_lock(&AccessBuffer3);
  alpha = buffer3[2];
  beta = buffer3[3];
  pthread_mutex_unlock(&AccessBuffer3);

  bitGenerator(alpha, low);
  bitGenerator(beta, high);

  for(ct = 0; ct < 18; ct ++)
    PanTiltStatus[ct] = 0;

  if(low[0] == 0 && high[0] == 0 && high[7] == 1)
    PanTiltStatus[0] = 1;
  if(low[0] == 0 && high[0] == 0 && high[6] == 1)
    PanTiltStatus[1] = 1;
  if(low[0] == 0 && high[0] == 0 && high[5] == 1)
    PanTiltStatus[2] = 1;
  if(low[0] == 0 && high[0] == 0 && high[4] == 1)
    PanTiltStatus[3] = 1;
  if(low[0] == 0 && high[2] == 0 && high[3] == 0)
    PanTiltStatus[4] = 1;
  if(low[0] == 0 && high[2] == 0 && high[3] == 1)
    PanTiltStatus[5] = 1;
  if(low[0] == 0 && high[2] == 1 && high[3] == 0)
    PanTiltStatus[6] = 1;
  if(low[0] == 0 && low[6] == 0 && low[7] == 0 && high[0] == 0)
    PanTiltStatus[7] = 1;
  if(low[0] == 0 && low[6] == 0 && low[7] == 1 && high[0] == 0)
    PanTiltStatus[8] = 1;
  if(low[0] == 0 && low[6] == 1 && low[7] == 0 && high[0] == 0)
    PanTiltStatus[9] = 1;
  if(low[0] == 0 && low[4] == 0 && low[5] == 0 && high[0] == 0)
    PanTiltStatus[10] = 1;
  if(low[0] == 0 && low[4] == 0 && low[5] == 1 && high[0] == 0)
    PanTiltStatus[11] = 1;
  if(low[0] == 0 && low[4] == 1 && low[5] == 0 && high[0] == 0)
    PanTiltStatus[12] = 1;
  if(low[0] == 0 && low[4] == 1 && low[5] == 1 && high[0] == 0)
    PanTiltStatus[13] = 1;
  if(low[0] == 0 && low[2] == 0 && low[3] == 0 && high[0] == 0)
    PanTiltStatus[14] = 1;
  if(low[0] == 0 && low[2] == 0 && low[3] == 1 && high[0] == 0)
    PanTiltStatus[15] = 1;
  if(low[0] == 0 && low[2] == 1 && low[3] == 0 && high[0] == 0)
    PanTiltStatus[16] = 1;
  if(low[0] == 0 && low[2] == 1 && low[3] == 1 && high[0] == 0)
    PanTiltStatus[17] = 1;
  return 1;
}

/*
 * pan: contain the pan max speed of the camera
 * tilt: containt the tilt max speed of the camera
 * Return 1 on success, 0 on error
 */
int EVILib::Pan_TiltMaxSpeedInq(int &pan, int &tilt)
{
  pan = -1;
  tilt = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d090610FF", Id_cam);
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
      pthread_mutex_lock(&TakeInfo);
      if(answer < 1)
	return 0;
      pthread_mutex_lock(&AccessBuffer3);
      pan = int(deconvert(2, 1));
      tilt = int(deconvert(3, 1));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * pan: contain the pan position of the camera
 * tilt: containt the tilt position of the camera
 * Return 1 on success, 0 on error
 */
int EVILib::Pan_TiltPosInq(float &pan, float &tilt)
{
  pan = -1.0;
  tilt = -1.0;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d090612FF", Id_cam);
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
      pthread_mutex_lock(&TakeInfo);
      if(answer < 1)
	return 0;
      pthread_mutex_lock(&AccessBuffer3);
      pan = deconvert(maxpan, maxPan, 2, 4);
      tilt = deconvert(maxtilt, maxTilt, 6, 4);
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * Return on success: EVILIB_POWER_ON_OFF
 *                    EVILIB_ZOOM_TELE_WIDE
 *                    EVILIB_AF_ON_OFF
 *                    EVILIB_CAM_BACKLIGHT
 *                    EVILIB_CAM_MEMORY
 *                    EVILIB_PAN_TILT_DRIVE
 *                    EVILIB_AT_MODE_ON_OFF
 *                    EVILIB_MD_MODE_ON_OFF
 * Return on error: 0
 */
int EVILib::IR_ReceiveReturn()
{
  char c, d;

  pthread_mutex_lock(&TakeReturn);
  pthread_mutex_lock(&AccessBuffer3);
  c = buffer3[4];
  d = buffer3[5];
  pthread_mutex_unlock(&AccessBuffer3);
// receiv 'information return'
  if(answer < 1)
    return 0;
  if(c == '\x04' && d == '\x00')
    return EVILIB_POWER_ON_OFF;
  if(c == '\x04' && d == '\x07')
    return EVILIB_ZOOM_TELE_WIDE;
  if(c == '\x04' && d == '\x38')
    return EVILIB_AF_ON_OFF;
  if(c == '\x04' && d == '\x33')
    return EVILIB_CAM_BACKLIGHT;
  if(c == '\x04' && d == '\x3F')
    return EVILIB_CAM_MEMORY;
  if(c == '\x06' && d == '\x01')
    return EVILIB_PAN_TILT_DRIVE;
//--------------------------------------------
// Only for D30
  if(c == '\x07' && d == '\x23')
    return EVILIB_AT_MODE_ON_OFF;
  if(c == '\x07' && d == '\x24')
    return EVILIB_MD_MODE_ON_OFF;
//--------------------------------------------
  return 0;
}
