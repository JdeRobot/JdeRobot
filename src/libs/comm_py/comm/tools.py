

def server2int(server):
    if isinstance(server, int):
        if server in range(3):
            return server
        return 0
    elif isinstance(server, str):
        serverl = server.lower()
        if serverl == "ice" or serverl == "1":
            return 1
        elif serverl == "ros" or serverl == "2":
            return 2
    else : return 0
