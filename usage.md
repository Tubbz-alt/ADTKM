# ADTKM Usage Guide
This guide will give examples for commonly used commands and features that come with the installed packages needed for the project. You can get a feel for the functionality here and either conduct your own tests or follow the [walkthrough](./walkthrough.md) we put together.

1. Refer to the installation and configuration guide to get everything set up properly. 
2. Set up cross-realm trust between the two servers. This will also require you to set up keytabs.
3. The wrapper can be used using `LD_PRELOAD`. 

## Enable cross-realm trust
On the two AD-DC boxes, use the following command:  
`samba-tool domain trust create DOMAIN [options]`  

**Helpful link:**  
https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/system-level_authentication_guide/using_trusts  

## Setting up the keytabs
Keytabs need to be exported in order to make sure permissions are properly granted for certain users to access certain services. Here is an example:  

**Create spn rcmd/beaglebone1.dtkm.local and rcmd/beaglebone1 for user BEAGLEBONE1$** (this is done on the AD-DC)  
`samba-tool spn add rcmd/beaglebone1.dtkm.local BEAGLEBONE1$`  
`samba-tool spn add rcmd/beaglebone1 BEAGLEBONE1$`  

### Export keytab
`samba-tool domain exportkeytab mykeytab-1 --principal=rcmd/beaglebone1.dtkm.local`  
`samba-tool domain exportkeytab mykeytab-1 --principal=rcmd/beaglebone1`  

Move to BBB, merge with /etc/krb5.keytab  
`ktutil copy mykeytab-1 /etc/krb5.keytab`  

run  
`kinit`  
`net ads join -k`  

To check, run  
`ktutil -k /etc/krb5.keytab list`  

## Edit CID files
Within the Relay Server and RTU folders, there is a subfolder called cidFiles. The files within contain references to the devices' IP and MAC addresses. You must change these to reflect your setup, otherwise the programs will not run correctly. 

## 61850 LD_PRELOAD
Download the code and change any hardcoded IP or FQDN references within to match your situation.  
Build with “make” command.  
Run with the following (same can be done for the server program):  
`LD_PRELOAD=/path/to/client.o ./binary `  
