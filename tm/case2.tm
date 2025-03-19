;states
#Q = {q0,q1,odd0,odd1,odd2,odd3,true1,true2,true3,true4,false1,false2,false3,false4,false5}

;input symbols
#S = {1}

;tape symbols
#G = {1,_,t,r,u,e,f,a,l,s}

;start state
#q0 = q0

;blank symbol
#B = _

;final state
#F = {true4}

;number of tapes
#N = 2

;start
q0 1_ 11 *r q1
q1 1_ 1_ *l odd0

;main part
odd0 11 11 *l odd0
odd0 1_ 1_ *r odd1
odd1 11 _1 rr odd1
odd1 1_ 1_ ** odd2
odd2 1_ 11 *r odd3
odd3 1_ 11 *l odd0
odd1 __ t_ r* true1
odd1 _1 f1 r* false1

;print true
true1 __ r_ r* true2
true2 __ u_ r* true3
true3 __ e_ r* true4

;print false
false1 _1 a1 r* false2
false2 _1 l1 r* false3
false3 _1 s1 r* false4
false4 _1 e1 r* false5