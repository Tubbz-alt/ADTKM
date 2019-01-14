# Experiment Walkthrough Guide
For this experiment, we are going to test the encrypted communication between two devices running relay servers and another device acting as an RTU, while simultaneously testing that cross-realm trust and authentication is properly being enforced via Samba policies and Kerberos, respectively.  

This guide will walk you through the steps needed to run the experiment, given that you have the required materials and have configured them correctly. The instructions for how to setup the devices can be found in hotlinks below.  

## Materials needed: 
* 3 Beaglebone Black devices, each with their own 61850 application license.  
* 2 devices running Samba server.  
* DNS server (can be done externally or through Samba).  

## Understanding the roles each device will play
One of the Samba servers will act as the central server, and the other as the field device. The configuration differences between the two Samba servers is limited to the server/realm names, which beaglebone devices join which server's domain, and the DNS records within the Samba settings should you choose to use Samba for DNS.  

Two of the beaglebone black devices will act as relay servers, where the third will be the RPC. The two relay servers will join the domain for the central server, while the RPC will join the domain for the field device.  

## Setting up the devices
### Set up two devices that will act as AD DCs running Samba Server. 
Instructions found [HERE](./installation.md#samba-server)  

### Set up three beaglebone black devices.
Instructions found [HERE](./installation.md#beaglebone-black)  

### Configure the device appropriately.  
Instructions found [HERE](./configuration.md)  

Once the Samba servers and beaglebone black devices are set up with all necessary tools installed and configured, continue.  

## Running the applications
### Starting the relay servers
On the two relay server devices, navigate to the `BeagleboneFiles/Relay_Server/lib/` folder.  
Use `LD_PRELOAD` when launching the server applications, referencing the `server.o` files in the `BeagleboneFiles/SASL_Wrapper/` folder.  
Example: `LD_PRELOAD=/BeagleboneFiles/SASL_Wrapper/server1.o ./server1_binary`

This will start the server applictions, where they will wait for the clients to commence the connection process before doing anything.  
Note: server1 and server2 are to be run on the two separate devices.

### Starting the clients
On the RTU device, navigate to the `BeagleboneFiles/RTU/lib/` folder.  
Use `LD_PRELOAD` when launching the client applications, referencing the `client.o` files in the `BeagleboneFiles/SASL_Wrapper/` folder.  
Example: `LD_PRELOAD=/BeagleboneFiles/SASL_Wrapper/client1.o ./client1_binary`

This will start the client applications, where they will look for the server applications that are awaiting their connection.  
Note: client1 and client2 are to be run on the same, single RTU device.  

### Starting the RTU server
On the RTU device, navigate to the `BeagleboneFiles/RTU/lib/` folder.  
Launch the RTU server application **without** `LD_PRELOAD`.  
Example: `./RTUServer_binary`  

This will start the RTU application.  

### Testing success
Once the applications are started, you can refresh the screen by hitting the `[Enter]` key on your keyboard, while the applications are the active program. If the client/server refresh to display the same values, that means they are properly communicating. If you ran the applications with the SASL wrapper (in other words, if you used `LD_PRELOAD`), that means the kerberos authentication was a success and you have cross-realm trust working correctly.  

You can use additional tools such as `tcpdump` to capture the traffic while the 61850 applications are running. Wireshark could then be used to investigate the contents of the output of `tcpdump`. 