#!/usr/bin/python3
# test

import paramiko
from importlib import import_module
#f_fn = import_module("fn1", package="f_fn")

t_host="oficina.estudioacoppo.com.ar"
n_port=10000
t_username="root"

#
ssh_client = paramiko.SSHClient()
ssh_client.load_system_host_keys()
ssh_client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
ssh_client.connect(t_host, username=t_username, port=n_port)
#f_fn(ssh_client)
ssh_client.close()
#

