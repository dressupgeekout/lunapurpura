# PRX Format Reverse Engineering

## Tue Jun  5 17:10:19 PDT 2018

There's a difference between stuff that the character writes themselves, and
stuff that other people write for the character.

For example, consider Viva (Tricky Decision):

    $ strings Viva.PRX | grep Tviv
    Tviv_j1_s1su
    Tviv_j2_s3_2s
    Tviv_j3_s3_3t
    Tviv_j4_s5b_1m
    Tviv_j5_s5b_2m
    Tviv_j6_s5b_2s
    Tviv_j7_s6a_2c
    Tviv_j8_s6a_3g
    Tviv_j9_s6b_1g    # <-- end of the 'j' series
    Tviv_n1_s1_1s     # <-- beginning of the 'n' series
    Tviv_n2_s3_3e
    Tviv_n3_s3_3t
    Tviv_n4_s3_3u
    Tviv_n5_s5b_2m
    Tviv_n6_s5b_2m
    Tviv_n7_s5b_2s
    Tviv_n8_s6a_1n
    Tviv_n9_s6a_1n
    Tviv_n10_s6a_2c
    Tviv_n11_s6a_3g
    Tviv_n12_s6b_1g

Here is `Tviv_j9_s6b_1g` (a diary entry from Viva):

```
TXT^@<CC>Tviv_j9_s6b_1g^@^@^@ ^@^@<B0>^B^@^@Well, dear diary, I have only
one thing to say: if you're going to be an Explorer in life (which Arrow,
Ginger and I definitely are), you shouldn't wear satin! I had a really good
time at DuBois Grille. Wolf's parents are the coolest adult people I ever
met. And of course everyone really liked my dead queen costume. But when one
of the waiters went by me, he tripped and got gravy all over my skirt! Arrow
tried to make me feel better about it by saying it made my costume look even
spookier, like it was blood. But you have to admit that was NOT thrillingly
romantic! And that waiter? He looked a lot like Arnold Zeitbaum -- but that
isn't possible, is it?^M
```

And here is `Tviv_n1_s1_1s` (a letter from Arrow to Viva):

```
TXT^@<D8>Tviv_n1_s1_1s^@^@^@^@^@^@}^A^@^@Hey Viva! ^MHappy Tuesday morning!
Only 4 more days till Wolf's party!! Soooooo-Meet me as soon as you can at
my locker after you get this. Because we need to discuss the Mission of the
Day:  Which is to find that ROCKETT PERSON!! I think she might be good help
on the costume thing, don't you? ^MWe HAVE TO do something totally major
2-KEWL!! ^M    Arrow^M
```

So the `j` probably stands for "journal" and `n` might stand for "note"?


-----

FILE FORMAT

* `SOH` (ASCII 1, "^A")
* (lots of zeroes!)
* `n`, `\0`, `n`, `\0`, `\0`, `\0`
