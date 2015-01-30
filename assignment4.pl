nonStopTrain(sandiego,oceanside).
nonStopTrain(lasvegas,sandiego).
nonStopTrain(sanfrancisco,bakersfield).
nonStopTrain(bakersfield,sandiego).
nonStopTrain(oceanside,losangeles).
nonStopTrain(portland,sanfrancisco).
nonStopTrain(seattle,portland).


canTravel(Src,Dest):-nonStopTrain(Src,Dest).
canTravel(Src,Dest):-(nonStopTrain(Src,A),canTravel(A,Dest))
; (nonStopTrain(Dest,B),canTravel(B,Src)).

person(sue,f).
person(jim,m).
person(tom,m).
person(joe,m).
person(cami,f).
person(bob,m).
person(fay,f).
person(beth,f).

likes(sue,yoga).
likes(jim,chess).
likes(tom,run).
likes(tom,yoga).
likes(joe,chess).
likes(joe,run).
likes(cami,chess).
likes(cami,yoga).
likes(bob,run).
likes(fay,yoga).
likes(fay,run).
likes(beth,chess).
likes(beth,run).

oppositegender(Name1,Name2):-person(Name1,Gender1),person(Name2,Gender2),not(Gender1==Gender2).
shareshobby(Name1,Name2):-likes(Name1,Hobby1),likes(Name2,Hobby2),Hobby1==Hobby2.

format([Person1,Person2,Person3,Person4,Person5,Person6,Person7,Person8|-]):-
	oppositegender(Person1,Person2),
	oppositegender(Person3,Person4),
	oppositegender(Person5,Person6),
	oppositegender(Person7,Person8),
	oppositegender(Person1,Person8),
	shareshobby(Person1,Person2),
	shareshobby(Person3,Person4),
	shareshobby(Person5,Person6),
	shareshobby(Person7,Person8).

front([Head|_],Head).

seatingChart(X):-
	N=[sue,jim,tom,joe,cami,bob,fay,beth],
	setof(Names,lists:perm(N,Names),Perms),
	seating_chart(Perms,NA),front(NA,X).

seating_chart([],_).
seating_chart([Head|Tail],X):-
	format(Head)->once(member(Head,X)),seating_chart(Tail,X)
	;seating_chart(Tail,X).






