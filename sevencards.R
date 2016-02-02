n = 10 #number of cards of each type?
cardvalues = c(1, 2, 4, 8, 16, 32, 64)
numcards = length(cardvalues) #number of different card types
cardcounts = rep(n, numcards)

N = 21 #target score
score = 0 #current score
cardsdrawn = 0 #number of cards drawn so far?
while(score < N) {
  whichcard = sample(1:numcards,1)
  score = score + cardvalues[whichcard]
  cardsdrawn = cardsdrawn + 1
}
