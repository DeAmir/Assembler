; every operation here is valid.

Name: .string "Amiros"
Movie: .string "Revenge of the C: homecoming"
Message: .string "hey"
Sales: .data 1,1,2

prn Message
red r7
prn r7
add #100,r7
prn r7
prn AvatarName

C: prn Movie
bne Name
not r3
cmp Sales, r3
 
.entry Name
.extern AvatarName
