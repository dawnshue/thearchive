signature SET =
sig
	type elem
	type set

	exception ElementNotFound

	val empty: set

	val insert: elem * set -> set

	val remove: elem * set -> set

	val member: elem * set -> bool
(*check fold*)
	val fold: ('a * elem -> 'a) -> ('a * set) -> 'a

	val union: set * set -> set

	val intersect: set * set -> set

	val setexists: (set -> bool) -> set -> bool

	val setall: (set -> bool) -> set -> bool

end

functor CreateSet (type x val equal: x * x -> bool) : SET =
struct
	type elem = x
	type set = elem list

	exception ElementNotFound

	val empty = []

	fun insert (item, []) = [item]
	|   insert (item, l::ls) = if equal(item, l) = true
				   then l::ls
				   else l::insert(item, ls)

	fun remove (item, []) = raise ElementNotFound
	|   remove (item, l::ls) = if equal(item, l) = true
				   then ls
				   else l::remove(item, ls)

	fun member (item, []) = false
	|   member (item, l::ls) = if equal(item, l) = true
				   then true
				   else member(item, ls)
(*check fold*)
	fun fold f (seed, []) = seed
	|   fold f (seed, l::ls) = fold f(f(seed, l), ls)

	fun union ([], l2) = l2
	|   union (l::ls, l2) = union(ls, insert(l, l2))

	fun intersect ([], l2) = []
	|   intersect (l::ls, l2) =
			if member(l, l2) = true
			then l :: intersect(ls, l2)
			else intersect(ls, l2)

	fun sub pred (ls, []) = pred ls
	|   sub pred (ls, h::ls2) =
			sub pred (insert(h, ls), ls2)
			orelse
			sub pred (ls, ls2)

	fun setexists pred [] = false
	|   setexists pred (l::ls) =
			sub pred (insert(l, []), ls)
			orelse
			setexists pred ls

	fun setall pred [] = true
	|   setall pred (l::ls) =
			sub pred (insert(l, []), ls)
			andalso
			setall pred ls

end
