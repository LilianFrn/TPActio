# TPActio

Lilian Fournier - Solène Altaber - Corentin Fraysse

** TP1: Commande MCC basique **  

On cherche à commander une MCC en PWM complémentaire décalé.
Pour cela on choisi les paramètre de configuration suivant :
- TIM1_CH1, TIM1_CH2, TIM1_CH1N, TIM1_CH2N seront nos 4 broches pour la PWM
- On a une fréquence de fonctionnement de 16 kHz = SysClk/(ARR+1)(PSC+1)=170 MHz/(1023+1)(9+1)
- On a un Dead Time à 220 ce qui correspond à 2 us
- Comme on veut 60% de duty cycle, on met un pulse de 613 pour le ch1 et 2

Etat d'avancement scéance 1: PWMs, EXTI et code commande générés 
Etape en cours : gestion de la vitesse

** TP2 **

- On rajoute le lancement des PWMs grâce à un bouton (GPIO_EXTI13). Pour cela, il suffit de lancer les fonctions HAL_TIM_PWM_Start et HAL_TIMX_PWM_Start dans la focntion de Callback de l'EXTI.
- On rajoute égalament le démarrage des PWM grace à une commande "start" dans le Shell. 
- On veut maintenant pouvoir gerer la vitesse du moteur (donc le rapport cyclique des PWMs) dans le Shell. 
Pour ce faire, il nous faut récuperer l'entrée manuscrite "speed = XXX" et modifier le registre CCR1 et 2 (counter) en conséquence.
Problème: On change brutalement le rapport cyclique ce qui crée un appel de courant important ce qui peut abimer le moteur.
Solution : on incrémente (ou décrémente) le rapport cyclique progressivement

Etat d'avancement scéance 2: PWMs, start en EXTI et UART et gestion de la vitesse dans le Shell
Etape en cours : relevé de courant 

** TP3 **

- Correction des lancements des PWM / câblage pour faire fonctionner le moteur
- Documentation Doxygen / nettoyage de code
- Début de l'acquisition du courant

Etat d'avancement scéance 3: Moteur et interface fonctionelle
Etape en cours : relevé de courant, asservissement



