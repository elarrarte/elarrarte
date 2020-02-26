#!/usr/bin/python

# INFORMACION DEL SCRIPT
# ---------------------------------------------------------
# AUTOR   : Ezequiel Larrarte
# FECHA   : 16-04-2019
# VERSION : 1
#
# DESCRIPCION
# Este script genera la firma digital en base
# a un template. La firma se puede generar en forma
# interactiva (via WEB) o de forma automatizada (via API)
# ---------------------------------------------------------

# LIBRERIAS
# ---------------------------------------------------------
import os
import sys
if('SCRIPTS' in os.environ.keys()):
    sys.path.append(os.environ['SCRIPTS'] + '/include')
import main
from flask import Flask, render_template, request, jsonify
import json
from posix_exitcodes import *
# ---------------------------------------------------------

# CONFIGURACION
# ---------------------------------------------------------
# ---------------------------------------------------------

# FUNCIONES
# ---------------------------------------------------------
app = Flask(__name__)

def render_signature(signature_data):
    t_signature = ""

    if\
    (
        not
        (
            't_acct_uid' in signature_data.keys() and
            't_acct_cn' in signature_data.keys() and
            't_acct_ou' in signature_data.keys() and
            't_acct_phone' in signature_data.keys() and
            't_acct_mobile' in signature_data.keys()
        )
    ):
        return(EXIT_FAILURE, t_signature)

    t_signature = render_template\
    (
        "signature.html",
        t_acct_uid=signature_data['t_acct_uid'],
        t_acct_cn=signature_data['t_acct_cn'],
        t_acct_ou=signature_data['t_acct_ou'],
        t_acct_phone=signature_data['t_acct_phone'],
        t_acct_mobile=signature_data['t_acct_mobile']
    )

    return(EXIT_SUCCESS, t_signature)

def output(t_format, t_status, t_msg):
    if(t_format == 'raw'):
        return(t_msg)

    if(t_format == 'json'):
        return(json.dumps({"t_status": t_status, "t_msg": t_msg}, indent=4) + '\n')

@app.route('/signature/api/create', methods=['POST'])
def create_signature():
    formats = ['raw', 'json']
    t_format = 'json'

    if(request.args.get('format')):
        if(request.args.get('format') in formats):
            t_format = request.args.get('format')
        else:
            return(output(t_format, EXIT_FAILURE, 'UNKNOWN FORMAT'))

    if(t_format == "json"):
        if(not request.json):
            return(output(t_format, EXIT_FAILURE, 'NOT A JSON REQUEST'))

        if\
        (
            "t_acct_uid" not in request.json.keys() or
            "t_acct_cn" not in request.json.keys() or
            "t_acct_ou" not in request.json.keys() or
            "t_acct_phone" not in request.json.keys() or
            "t_acct_mobile" not in request.json.keys()
        ):
            return(output(t_format, EXIT_FAILURE, 'INVALID API REQUEST'))

        signature_data =\
        {
            't_acct_uid': request.json['t_acct_uid'],
            't_acct_cn': request.json['t_acct_cn'],
            't_acct_ou': request.json['t_acct_ou'],
            't_acct_phone': request.json['t_acct_phone'],
            't_acct_mobile': request.json['t_acct_mobile']
        }

    n_ec, t_signature = render_signature(signature_data)
    if(n_ec):
        return(output(t_format, EXIT_FAILURE, "ERROR render_signature()"))
    else:
        return(output(t_format, EXIT_SUCCESS, t_signature))

@app.route("/signature")
def signature():
    return(render_template('form.html'))
# ---------------------------------------------------------

# PRINCIPAL
# ---------------------------------------------------------
if __name__ == "__main__":
    # app.run(ssl_context='adhoc')
    app.run()
# ---------------------------------------------------------


