# ADTKM Usage Guide

1. Refer to the installation and configuration guide to get everything set up properly. 
2. Set up cross-realm trust between the two servers. This will also require you to set up keytabs.
3. The wrapper can be used using LD_PRELOAD. 

## Enable cross-realm trust
On the two AD-DC boxes, use the following command: 
<span style="font-family:Courier New;">
samba-tool domain trust create DOMAIN [options]
</span>

**Helpful link:**
https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/system-level_authentication_guide/using_trusts



## Setting up the keytabs
Keytabs need to be exported in order to make sure permissions are properly granted for certain users to access certain services. Here is an example: 

Create spn rcmd/beaglebone1.dtkm.local and rcmd/beaglebone1 for user BEAGLEBONE1$ (this is done on the AD-DC)
<span style="font-family:Courier New;">samba-tool spn add rcmd/beaglebone1.dtkm.local BEAGLEBONE1$</span>
<span style="font-family:Courier New;">samba-tool spn add rcmd/beaglebone1 BEAGLEBONE1$</span>

### Export keytab
<span style="font-family:Courier New;">
samba-tool domain exportkeytab mykeytab-1 --principal=rcmd/beaglebone1.dtkm.local
samba-tool domain exportkeytab mykeytab-1 --principal=rcmd/beaglebone1
</span>

Move to BBB, merge with /etc/krb5.keytab
<span style="font-family:Courier New;">
ktutil copy mykeytab-1 /etc/krb5.keytab
</span>

run 
<span style="font-family:Courier New;">
kinit 
net ads join -k
</span>

To check, run 
<span style="font-family:Courier New;">
ktutil -k /etc/krb5.keytab list
</span>


## 61850 LD_PRELOAD
Download the code and change any hardcoded IP or FQDN references within to match your situation. 
Build with “make” command. 
Run with the following (same can be done for the server program):
<span style="font-family:Courier New;">
LD_PRELOAD=/path/to/client.o ./binary 
</span>


