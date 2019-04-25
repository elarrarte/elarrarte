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
sys.path.append(os.environ['SCRIPTS'] + '/include')
import main
from flask import Flask, render_template, request, jsonify
import json
# ---------------------------------------------------------

# CONFIGURACION
# ---------------------------------------------------------
# ---------------------------------------------------------

# FUNCIONES
# ---------------------------------------------------------
app = Flask(__name__)

#@app.route('/api/signature/<t_acct_uid>/<t_acct_cn>/<t_acct_ou>/<t_acct_phone>/<t_acct_mobile>')
#def create_signature(t_acct_uid, t_acct_cn, t_acct_ou, t_acct_phone, t_acct_mobile):
#    t_status = 'OK'
#    t_signature = render_template('signature.html',
#        t_acct_uid=t_acct_uid, t_acct_cn=t_acct_cn, t_acct_ou=t_acct_ou,
#        t_acct_phone=t_acct_phone, t_acct_mobile=t_acct_mobile)
#    j_reply = json.dumps({"status": t_status, "signature": t_signature})
#    return(j_reply)

def dict_reply(t_status, t_msg):
    return({"status": t_status, "msg": t_msg})

@app.route('/api/signature', methods=['POST'])
def create_signature():
    if(not request.json):
        d_reply = dict_reply('ERROR', 'NOT A JSON REQUEST')
        return(json.dumps(d_reply, indent=4) + '\n')

    if("t_acct_uid" not in request.json.keys() or
        "t_acct_cn" not in request.json.keys() or
        "t_acct_ou" not in request.json.keys() or
        "t_acct_phone" not in request.json.keys() or
        "t_acct_mobile" not in request.json.keys()):
        d_reply = dict_reply('ERROR', 'INVALID API REQUEST')
        return(json.dumps(d_reply, indent=4) + '\n')

    d_reply = dict_reply('OK', 'SIGNATURE CREATED')
    d_reply["signature"] = render_template('signature.html',
        t_acct_uid=request.json['t_acct_uid'],
        t_acct_cn=request.json['t_acct_cn'],
        t_acct_ou=request.json['t_acct_ou'],
        t_acct_phone=request.json['t_acct_phone'],
        t_acct_mobile=request.json['t_acct_mobile'])
    return(json.dumps(d_reply, indent=4) + '\n')

@app.route("/signature")
def signature():
    return(render_template('form.html'))
# ---------------------------------------------------------

# PRINCIPAL
# ---------------------------------------------------------
# ---------------------------------------------------------


