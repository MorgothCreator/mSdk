;******************************************************************************

;****************************** Global Symbols*********************************
        .global omap_smc_rom

;**************************** Text Section ************************************
        .text

; This source file is assembled for ARM instructions
        .state32

;*****************************************************************************
omap_smc_rom:
    push {r4-r11,lr}
    mov    r12,    r0
    mov    r0,    r1
    dsb
    isb
    dmb
    /*smc      #0*/
    pop {r4-r11,pc}
;
; End of the file
;
    .end
