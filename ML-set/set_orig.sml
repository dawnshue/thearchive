use "set.sml";

(* Tests set.sml with a unique structure *)

structure mytup =
struct
	type x = { a : int, b : int }

	fun eql ({a=aa,b=bb}, {a=aa2,b=bb2}) =
		if aa - aa2 = 0 andalso bb - bb2 = 0
		then true
		else false

	val equal = eql

end;

structure t = CreateSet (mytup);

open t;

(* EMPTY *)
val lt = t.empty;

(* INSERT *)
val lt = t.insert ({a=1,b=1}, lt);
val lt = t.insert ({a=1,b=1}, lt);
val lt = t.insert ({a=2,b=2}, t.insert({a=3,b=3}, lt));

(* REMOVE *)
val r = t.remove ({a=3,b=3}, lt);
(* val lt = t.remove ({a=1,b=2}, lt); *) (*raises ElementNotFound*)

(* MEMBER *)
val m = t.member ({a=2,b=2}, r);
val m = t.member ({a=3,b=3}, r);

(* FOLD *)
	val adder =
		fn ({a=i,b=j}, {a=ii,b=jj}) =>
					{a=i+ii, b=j+jj};
val f = t.fold adder ({a=10,b=10}, r);

(* UNION *)
val u = t.union (r, r);
val u2 = t.union (lt, r);

(* INTERSECT *)
val i = t.intersect (r, lt);

(* SETEXISTS *)
	fun even2 {a=m,b=n} =
		if m mod 2 = 0 andalso n mod 2 = 0
		then true
		else false
	fun even [] = false
	|   even (l::ls) =
		even2 l
		orelse even ls;
val se = t.setexists even lt;
	val odd = t.insert (f, t.empty);
val se = t.setexists even odd;

(* SETALL *)
val sa = t.setall even lt;
	val ev = t.insert ({a=2,b=2},t.empty);
val sa = t.setall even ev;

