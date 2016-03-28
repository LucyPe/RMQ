Paramtre: <alg_mode> <input_mode> <n> <m> <other_params>

<alg_mode>:
	{'Triv', 'Big', 'FH', 'Fast', 'Fast2', 'Seg', 'SegA'} - spustí konkrétny algoritmus a vypíše výsledky (pozície minima) a čas potrebný na predspracovanie a na zodpovedanie danej sady dotazov
 	'c' - spustí všetky algoritmy a porovná výsledky s triviálnym algoritmom a vypíše počet chýb pre každý algoritmus
	'p' - spustí všetky algoritmy a vypíše priemerný čas potrebný na predspracovanie a na zodpovedanie danej sady dotazov pre každý algoritmus

<input_mode>:
	'f' - načítanie vstupu zo súborov
	'g' - vygenerovanie vstupu

<n> : dĺžka postupnosti
<m> : počet dotazov

<other_params>:
	<file_path1> <file_path2> - cesty k súborom s postupnosťou a dotazmi pre mód 'f'
	<k> - dĺžka každého dotazu pre mód 'g'
