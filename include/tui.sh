# FUNCIONES
# ---------------------------------------------------------

# EXIT CODES
TUI_ACTION_OK=0
TUI_ACTION_CANCEL=1
TUI_ACTION_YES=0
TUI_ACTION_NO=1

function tui_init()
{
    t_tui_tmpfile=$(mktemp)
}

function tui_clean()
{
    rm -f ${t_tui_tmpfile}
}

function tui_msgbox()
{
    local t_title
    local t_text
    local n_y
    local n_x

    t_title=${1}
    t_text=${2}
    n_y=${3}
    n_x=${4}

    [ -z "${t_title}" ] && t_title='---'

    whiptail --clear --title "${t_title}" --msgbox "${t_text}" ${n_y} ${n_x}
}

function tui_inputbox()
{
    local t_title
    local t_text
    local n_y
    local n_x
    local t_def

    t_title=${1}
    t_text=${2}
    n_y=${3}
    n_x=${4}
    t_def=${5}

    [ -z "${t_title}" ] && t_title='---'

    whiptail --clear --title "${t_title}" --inputbox "${t_text}" \
		${n_y} ${n_x} ${t_def} 2> ${t_tui_tmpfile}
    fr_n_action=$?
    fr_t_input=$(cat ${t_tui_tmpfile})
}

function tui_textbox()
{
    local t_title
    local n_y
    local n_x
    local t_file

    t_title=${1}
    n_y=${2}
    n_x=${3}
    t_file=${4}

    [ -z "${t_title}" ] && t_title='---'

    whiptail --clear --title "${t_title}" --scrolltext \
		--textbox "${t_file}" ${n_y} ${n_x}
}

function tui_menu()
{
    local t_title
    local t_text
    local n_y
    local n_x
    local n_list_y
    local t_items

    t_title=${1}
    t_text=${2}
    n_y=${3}
    n_x=${4}
    n_list_y=${5}
    t_items=${6}

    [ -z "${t_title}" ] && t_title='---'

    whiptail --clear --nocancel --title "${t_title}" --menu "${t_text}" \
		${n_y} ${n_x} ${n_list_y} ${t_items} 2> ${t_tui_tmpfile}
    fr_t_opt=$(cat ${t_tui_tmpfile})
}

function tui_checklist()
{
    local t_title
    local t_text
    local n_y
    local n_x
    local n_list_y
    local t_items

    t_title=${1}
    t_text=${2}
    n_y=${3}
    n_x=${4}
    n_list_y=${5}
    t_items=${6}

    [ -z "${t_title}" ] && t_title='---'

    whiptail --clear --title "${t_title}" --checklist "${t_text}" \
		${n_y} ${n_x} ${n_list_y} ${t_items} 2> ${t_tui_tmpfile}
    fr_n_action=$?
    fr_t_opts=$(cat ${t_tui_tmpfile})
}

function tui_radiolist()
{
    local t_title
    local t_text
    local n_y
    local n_x
    local n_list_y
    local t_items

    t_title=${1}
    t_text=${2}
    n_y=${3}
    n_x=${4}
    n_list_y=${5}
    t_items=${6}

    [ -z "${t_title}" ] && t_title='---'

    whiptail --clear --title "${t_title}" --radiolist "${t_text}" \
		${n_y} ${n_x} ${n_list_y} ${t_items} 2> ${t_tui_tmpfile}
    fr_n_action=$?
    fr_t_opt=$(cat ${t_tui_tmpfile})
}

# ---------------------------------------------------------
