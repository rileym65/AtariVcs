vsync      equ    $00      ; vertical sync
vblank     equ    $01      ; Vertial blank/latched port enable
wsync      equ    $02      ; Wait for horizontal blank
nusiz0     equ    $04      ; Number size, Player/Missile 0
nusiz1     equ    $05      ; Number size, Player/Missile 1
colup0     equ    $06      ; Color - Player/Missile 0
colup1     equ    $07      ; Color - Player/Missile 1
colupf     equ    $08      ; Color - Playfield/Ball
colubk     equ    $09      ; Color - Background
ctrlpf     equ    $0a      ; Control Playfield, Ball
refp0      equ    $0b      ; Reflect Player 0
refp1      equ    $0c      ; Reflect Player 1
pf0        equ    $0d      ; Playfield 0
pf1        equ    $0e      ; Playfield 1
pf2        equ    $0f      ; Playfield 2
resp0      equ    $10      ; Reset player 0
resp1      equ    $11      ; Reset player 1
resm0      equ    $12      ; Reset missile 0
resm1      equ    $13      ; Reset missile 1
resbl      equ    $14      ; Reset ball
audc0      equ    $15      ; Audio control channel 0
audc1      equ    $16      ; Audio control channel 1
audf0      equ    $17      ; Audio frequency channel 0
audf1      equ    $18      ; Audio frequency channel 1
audv0      equ    $19      ; Audio volume channel 0
audv1      equ    $1a      ; Audio volume channel 1
grp0       equ    $1b      ; Graphics bitmap player 0
grp1       equ    $1c      ; Graphics bitmap player 1
enam0      equ    $1d      ; Enable missile 0
enam1      equ    $1e      ; Enable missile 1
enabl      equ    $1f      ; Enable ball
hmp0       equ    $20      ; Horizontal motion player 0
hmp1       equ    $21      ; Horizontal motion player 1
hmm0       equ    $22      ; Horizontal motion missile 0
hmm1       equ    $23      ; Horizontal motion missile 1
hmbl       equ    $24      ; Horizontal motion ball
vdelp0     equ    $25      ; Vertical delay player 0
vdelp1     equ    $26      ; Vertical delay player 1
vdelbl     equ    $27      ; Vertical delay ball
resmp0     equ    $28      ; Reset missile 0 to player 0
resmp1     equ    $29      ; Reset missile 1 to player 1
hmove      equ    $2a      ; Apply horizontal motion
hmclr      equ    $2b      ; Clear horizontal motion registers
cxclr      equ    $2c      ; Clear collision latches
cxm0p      equ    $30      ; Collision m0-p1, m0-p0
cxm1p      equ    $31      ; Collision m1-p0, m1-p1
cxp0fb     equ    $32      ; Collision p0-pf, p0-bl
cxp1fb     equ    $33      ; Collision p1-pf, p1-bl
cxm0fb     equ    $34      ; Collision m0-pf, m0-bl
cxm1fb     equ    $35      ; Collision m1-pf, m1-bl
cxblpf     equ    $36      ; Collision bl-pf
cxppmm     equ    $37      ; Collision p0-p1, m0-m1
inpt0      equ    $38      ; Dumped input port 0
inpt1      equ    $39      ; Dumped input port 1
inpt2      equ    $3a      ; Dumped input port 2
inpt3      equ    $3b      ; Dumped input port 3
inpt4      equ    $3c      ; Latched input port 4
inpt5      equ    $3d      ; Latched input port 5
swcha      equ    $280     ; Joysticks/Controllers
swacnt     equ    $281     ; Port A DDR
swchb      equ    $282     ; Console switches
swbcnt     equ    $283     ; Port B DDR
intim      equ    $284     ; Timer output
tim1t      equ    $294     ; Set 1 cycle timer
tim8t      equ    $295     ; Set 8 cycle timer
tim64t     equ    $296     ; Set 64 cycle timer
tim1024t   equ    $297     ; Set 1024 cycle timer

vsync_on   equ    $02      ; Turn on vsync
vsync_off  equ    $00      ; Turn off vsync
pf_reflect equ    $01      ; Playfield reflect mode
pf_score   equ    $02      ; Playfied scoreboard mode
pf_pripf   equ    $04      ; Playfield has priority
pl_reverse equ    $08      ; Reverse player sprite
en_ball    equ    $02      ; Enable ball
en_missile equ    $02      ; Enable missile
ballsz1    equ    $00      ; Ball size 1 pixel
ballsz2    equ    $10      ; Ball size 2 pixels
ballsz4    equ    $20      ; Ball size 4 pixels
ballsz8    equ    $30      ; Ball size 8 pixels
pl_double  equ    $05      ; Player double width
pl_quad    equ    $07      ; Player quad width
pl_2_cl    equ    $01      ; Two copies close
pl_2_md    equ    $02      ; Two copies medium
pl_3_cl    equ    $03      ; Three copies close
pl_2_wd    equ    $04      ; Two copies wide
pl_3_md    equ    $06      ; Three copies medium

dbg_clrctr equ    $00      ; Clear cycle counter
dbg_prtctr equ    $01      ; Print cycle counter without clear
dbg_prtclr equ    $02      ; Print cycle counter and clear
dbg_sldc   equ    $03      ; Print scan line and dot clock
dbg_cpu    equ    $04      ; Show cpu registers
dbg_tia    equ    $05      ; Show TIA registers
dbg_number equ    $06      ; Show following number
dbg_nl     equ    $0c      ; Output new line
dbg_exit   equ    $ff      ; Exit emulator

