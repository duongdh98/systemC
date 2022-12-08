# How to build systemC in linux

- Download SystemC source from Accellera website.
- Untar the package using tar -xzvf systemc-2.3.0.tgz
- Change to the top level directory systemc-2.3.0 : cd systemc-2.3.0
- Make a directory systemc for installation in your /usr/local/ path.

```sh
    sudo mkdir /usr/local/systemc
```

- Create a directory for object code using mkdir objdir
- Change directory to objdir using cd objdir
- Run configure from your top level directory: ../configure --prefix=/usr/local/systemc
- Install by typing command: sudo make install

Save the file as hello.cpp. Use following command to export a variable SYSTEMC_HOME.

```sh
    export SYSTEMC_HOME=/usr/local/systemc230/
```

Now you can use following command to compile the program:

```sh
g++ -I. -I$SYSTEMC_HOME/include -L. -L$SYSTEMC_HOME/lib-linux -Wl,-rpath=$SYSTEMC_HOME/lib-linux -o hello hello.cpp -lsystemc -lm
```