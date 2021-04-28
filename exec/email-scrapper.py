#!/usr/bin/python

# INFORMACION DEL SCRIPT
# ---------------------------------------------------------
# AUTOR   : Ezequiel Larrarte
# FECHA   : 08-12-2020
# VERSION : XX
#
# DESCRIPCION
# Email scrapper
# ---------------------------------------------------------

# LIBRERIAS
# ---------------------------------------------------------
import os
import sys
if "SCRIPTS" in os.environ.keys():
    sys.path.append(os.environ['SCRIPTS'] + '/include')
from posix_exitcodes import *
import main
import requests
import re
# ---------------------------------------------------------

# CONFIGURACION
# ---------------------------------------------------------
url="http://www.gigot.com.ar/"
max_urls=10
max_emails=10
# ---------------------------------------------------------

# FUNCIONES
# ---------------------------------------------------------
# ---------------------------------------------------------

# PRINCIPAL
# ---------------------------------------------------------

urls = 0
emails = 0

while more_urls < max_urls and emails < max_emails:
    reply = requests.request(url=url, method="GET")

exit(EXIT_SUCCESS)
# ---------------------------------------------------------
