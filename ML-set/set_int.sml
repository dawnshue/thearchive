use "set.sml";

fun eql (a, b) =
	if (a - b) = 0
	then true
	else false

structure t = CreateSet (type x = int val equal = eql);

open t;

(* EMPTY *)
val lt = t.empty;

(* INSERT *)
val lt = t.insert (10, lt);
val lt = t.insert (15, t.insert(20, lt));

(* REMOVE *)
val r = t.remove (15, lt);
(* val lt = t.remove (5, lt); *) (*raises ElementNotFound*)

(* MEMBER *)
val m = t.member (15, r);
val m = t.member (10, r);

(* FOLD *)
	val adder = fn (a, b) => a + b;
val f = t.fold adder (0, r);

(* UNION *)
val u = t.union (lt, lt);
	val lt2 = t.insert (1, t.insert(2, t.insert(20, t.empty)));
val u2 = t.union (r, lt2);

(* INTERSECT *)
val i = t.intersect (r, lt2);

(* SETEXISTS *)
	fun even [] = false
	|   even (l::ls) =
		if l mod 2 = 0
		then true
		else even ls;
val se = t.setexists even lt;
	val odd = t.insert (15, t.empty);
val se = t.setexists even odd;

(* SETALL *)
val sa = t.setall even lt;
val sa = t.setall even r;
