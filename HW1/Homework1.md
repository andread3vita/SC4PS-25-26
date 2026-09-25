# How to set up a Linux machine on CloudVeneto

This guide describe a step-by-step solution to set up a functioning working enviroment on CloudVeneto

##  Create a virtual machine on CloudVeneto

 Go to http://cloud-areapd.pd.infn.it/ and log in using your credentials (both UniPD and INFN account works)

 Under the identity menu join the `SC4PS-PhD ` project

> If not already done create a key pair in the relative menu

 Create a new instance in the relative menu, a good set of setting is:
  - Origin: `AlmaLinux10-INFNPadova-2026-02-17`
  - Flavour: `cldareapd.large`
  - IMPORTANT: asign a key that you have access to

## Access your virtual machine from terminal 

> **If not already done:** protect your key file using
>```
>chmod 600 /path/to/your/key
>```

 Connect using ssh, passing through @gate.cloudveneto.it

``` 
ssh -J USERNAME@gate.cloudveneto.it -i /path/to/your/key almalinux@IP_ADDRESS_OF_YOUR_MACHINE
```
> Note: `almalinux` is the default user for for AlmaLinux images, can be different if you chose a different image

> The same ssh line can be used to set up VS Code

## Install the C Compiler (GCC)

 The first time you access the machine it is good practice to update the system and package list:

 ```
sudo dnf check-update
sudo dnf upgrade -y
```

To compile C/C++ code on AlmaLinux we need the "Development Tools" and CRB (Code Ready Builder) repository. This will install the `gcc` compiler, the `make` tool, and other usefull libraries.

```
sudo dnf groupinstall "Development Tools" -y
sudo dnf config-manager --set-enabled crb
```

We can verify the installation of the compiler:

```
gcc --version
```



