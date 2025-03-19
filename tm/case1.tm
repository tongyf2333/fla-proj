;set of states
#Q = {q0,copy1,copy2,clear,itera,reseta,write,i0,i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11,i12,i13}

;set of input symbols
#S = {a,b}

;set of tape symbols
#G = {a,b,c,_,i,l,e,g,a,n,p,u,t}

;start state
#q0 = q0

;blank symbol
#B = _

;final states
#F = {write}

;number of tapes
#N = 4

;transition functions
;old_state old_symbol new_symbol direction new_state

;starter
q0 *___ *___ **** copy1

;copy a and b
copy1 a___ _a__ rr** copy1
copy1 b___ __b_ r*r* copy2
copy2 b___ __b_ r*r* copy2
copy2 a___ ____ r*** clear
copy2 ____ ____ *ll* itera

;generating c
itera _ab_ _abc *l*l itera
itera __b_ __b_ *r** reseta
reseta _ab_ _ab_ *r** reseta
reseta __b_ __b_ *ll* itera
itera _a__ _a__ ***r write
write _a_c ca_c r**r write

;clearing
clear a___ ____ r*** clear
clear b___ ____ r*** clear
clear ____ ____ r*** i0

;write illegal input
i0 ____ i___ r*** i1
i1 ____ l___ r*** i2
i2 ____ l___ r*** i3
i3 ____ e___ r*** i4
i4 ____ g___ r*** i5
i5 ____ a___ r*** i6
i6 ____ l___ r*** i7
i7 ____ ____ r*** i8
i8 ____ i___ r*** i9
i9 ____ n___ r*** i10
i10 ____ p___ r*** i11
i11 ____ u___ r*** i12
i12 ____ t___ r*** i13
