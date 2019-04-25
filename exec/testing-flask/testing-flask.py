#!/usr/bin/python

# INFORMACION DEL SCRIPT
# ---------------------------------------------------------
# AUTOR   : Ezequiel Larrarte
# FECHA   : 16-04-2019
# VERSION : 1
#
# DESCRIPCION
# Pruebas con el framework [Flask]
# ---------------------------------------------------------

# LIBRERIAS
# ---------------------------------------------------------
import os
import sys
sys.path.append(os.environ['SCRIPTS'] + '/include')
import main
from flask import Flask
from flask import render_template
# ---------------------------------------------------------

# CONFIGURACION
# ---------------------------------------------------------
# ---------------------------------------------------------

# FUNCIONES
# ---------------------------------------------------------
app = Flask(__name__)

@app.route("/")
def index():
    return('Testing Flask!')

@app.route("/template/<name>")
def template(name=None):
    return(render_template('template.html', name=name))

@app.route("/signature")
def signature():
    return(render_template('form.html'))
# ---------------------------------------------------------

# PRINCIPAL
# ---------------------------------------------------------
# ---------------------------------------------------------


