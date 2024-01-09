mov r2, 0
ifz r2 _label_one
halt

_label_one:
    call _label_two
    halt

_label_two:
    inc r2
    ret
