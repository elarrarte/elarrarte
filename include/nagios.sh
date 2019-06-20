# NAGIOS PLUGIN EXIT CODES
NAGIOS_EXIT_OK=0
NAGIOS_EXIT_WARNING=1
NAGIOS_EXIT_CRITICAL=2
NAGIOS_EXIT_UNKNOWN=3

function exit_ok()
{
    local t_msg
    t_msg=${1}
    echo "OK - ${t_msg}"
    exit ${NAGIOS_EXIT_OK}
}

function exit_warning()
{
    local t_msg
    t_msg=${1}
    echo "WARNING - ${t_msg}"
    exit ${NAGIOS_EXIT_WARNING}
}

function exit_critical()
{
    local t_msg
    t_msg=${1}
    echo "CRITICAL - ${t_msg}"
    exit ${NAGIOS_EXIT_CRITICAL}
}

function exit_unknown()
{
    local t_msg
    t_msg=${1}
    echo "UNKNOWN - ${t_msg}"
    exit ${NAGIOS_EXIT_UNKNOWN}
}

