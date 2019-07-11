#!/usr/bin/python3

# INFORMACION DEL SCRIPT
# ---------------------------------------------------------
# AUTOR   : Ezequiel Larrarte
# FECHA   : 10-07-2019
# VERSION : 1
#
# DESCRIPCION
# Este script funciona como middleware a la API de Telegram
# recibiendo peticiones HTTP y transformandolas a peticiones
# HTTPS, permitiendo la comunicacion de aplicaciones que no
# soportan el protocolo HTTPS.
# ---------------------------------------------------------

# LIBRERIAS
# ---------------------------------------------------------
import os
import sys
import main
from flask import Flask
from flask import request
import requests
# ---------------------------------------------------------

# CONFIGURACION
# ---------------------------------------------------------
# ---------------------------------------------------------

# FUNCIONES
# ---------------------------------------------------------
app = Flask(__name__)

# Telegram API Authentication
T_API_URL = "https://api.telegram.com"
T_API_BOT = "bot544133319"
T_API_KEY = "AAFFQYI5HTqJLGPk7BiOGcEywHqgIhYf4L8"

# Middleware API send_message call
@app.route("/send_message", methods=['GET', 'POST'])
def send_message():
    print(request.args.get('chat_id'))
    print(request.args.get('text'))
    return("send_message()")
# ---------------------------------------------------------

# PRINCIPAL
# ---------------------------------------------------------
if __name__ == "__main__":
    app.run()
# ---------------------------------------------------------


