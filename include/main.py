# LIBRERIAS
# ---------------------------------------------------------
from posix_exitcodes import *
import requests
# ---------------------------------------------------------

# FUNCIONES
# ---------------------------------------------------------

# FUNCION get_json()
# ---------------------------------------------------------
#
# DESCRIPCION
# Esta funcion realiza una consulta HTTP con metodo GET
# y retorna un objeto JSON
#
# PARAMETROS
# t_url     : URL
# d_headers : diccionario con los encabezados HTTP
# t_data    : datos a enviar
#
# RETORNO
# reply["exitcode"]
#   EXIT_SUCCESS  : consulta OK
#   EXIT_FAILURE  : consulta ERROR
# reply["status"] : mensaje de estado
# reply["data"]   : respuesta en formato JSON
def get_json(t_url, d_headers, t_data):
    session = requests.Session()
    session.headers.update(d_headers)
    reply = {}

    try:
        req_reply = session.get(t_url, data=t_data)
    except Exception as exception:
        reply["exitcode"] = EXIT_FAILURE
        reply["status"] = "EXCEPTION: " + str(type(exception))
        reply["data"] = "-"

    reply["status"] = "HTTP-CODE [" + str(req_reply.status_code) + "]"
    if req_reply.status_code == requests.codes.ok:
        reply["exitcode"] = EXIT_SUCCESS
        reply["data"] = req_reply.json()
    else:
        reply["exitcode"] = EXIT_FAILURE
        reply["data"] = "-"

    return(reply)
# ---------------------------------------------------------

# ---------------------------------------------------------

