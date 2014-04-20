use "subss.sml";
(* Tests subset functionality of subss.sml with int type *)

fun eql (a, b) =
	if (a - b) = 0
	then true
	else false

structure t = CreateSet (type x = int val equal = eql);

open t;

(* CREATE SET *)
val lt = t.empty;
val lt = t.insert (11, lt);
val lt = t.insert (15, t.insert(~11, lt));

(* PREDICATE *)
	fun sum [] = 0
	|   sum (l::ls) =
		l + (sum ls)
	fun psum set =
		if sum set = 0
		then true
		else false;

(* TEST SUBSETSUM *)
(* test TRUE case *)
val ltexists = t.setexists psum lt;
val ltsub = t.subsetsum psum lt;

(* test FALSE case *)
val lt = t.remove (~11, lt);
val ltexists = t.setexists psum lt;
val ltsub = t.subsetsum psum lt;
