# SASL Wrapper
This guide will teach you everything you need to know about the SASL wrapper we developed and how to use it.  

## Functionality
The functionality of the wrapper is to ensure that two applications (a client and a server) have successfully passed an authentication test via Kerberos before allowing them to communicate. Hooking/injection is being used to intercept system calls, such as `send` and `recv`. It was developed to work in a Linux-based environment.  

## What is Necessary
For the wrapper to agree that authentication between the devices was successful, a few things need to be set up properly.  

### Install and Configure Kerberos  
Both devices have installed and configured Kerberos. We used [Heimdal](https://www.h5l.org/)

### Configure Keytabs
Keytabs need to be exported in order to make sure permissions are properly granted for certain users to access certain services. Here is an example using Samba:  

#### Create spn rcmd/beaglebone1.dtkm.local and rcmd/beaglebone1 for user BEAGLEBONE1$
`samba-tool spn add rcmd/beaglebone1.dtkm.local BEAGLEBONE1$`  
`samba-tool spn add rcmd/beaglebone1 BEAGLEBONE1$`  

#### Export the keytab
`samba-tool domain exportkeytab mykeytab-1 --principal=rcmd/beaglebone1.dtkm.local`  
`samba-tool domain exportkeytab mykeytab-1 --principal=rcmd/beaglebone1`  

#### Merge the keytab with /etc/krb5.keytab  
`ktutil copy mykeytab-1 /etc/krb5.keytab`  

#### Verify
`ktutil -k /etc/krb5.keytab list`  

### Cross-Realm Trust
If the client/server exist on separate domains/realms, cross-realm trust needs to be established between them.  
In our case, we used Samba as an AD-DC and the command to create a trust was as follows:  
`samba-tool domain trust create DOMAIN [options]`  

For more info on this, take a look at this helpful link:  
https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/system-level_authentication_guide/using_trusts

## SASL Wrapper Files
The code for the wrapper can be found [HERE](./BeagleboneFiles/SASL Wrapper/)  
This folder has all that is needed to get started.  

## Usage
1. Edit the contents of the files and modify the host variable as necessary. In both the `wrapper.c` and `wrapper_s.c` files, the host variable should be a string that contains the fqdn of the device that the server will be running on.  
2. Use the `make` command to generate the the `client.o` and `server.o` files.  
3. Use `LD_PRELOAD` when executing your server/client applications to use the wrapper.  
Example: `LD_PRELOAD=/Path/To/server.o /Path/To/server_binary`  
