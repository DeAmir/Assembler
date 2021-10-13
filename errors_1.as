; every line contains some kind of error.
; label errors:
AAAAAAAAAABBBBBBBBBBCCCCCCCCCCDD: .data -22
3p: .data 4
non%s: .string "h"
r7: .data 5
add: .string "popolopo"

;double declaration
MOMO: .data 1
MOMO: .data 2


;operation errors
prn #5, r4
add r4,#5
add ,r1,r1
add r2,,r2
add r3,r3,

; instruction errors
.string "more than 2 ""
.string
.data lo,9
.data 1,
