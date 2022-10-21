# TPActio

Lilian Fournier - Solène Altaber - Corentin Fraysse

** TP1: Commande MCC basique **  

On cherche à commander une MCC en PWM complémentaire décalé.
Pour cela on choisi les paramètre de configuration suivant :
- TIM1_CH1, TIM1_CH2, TIM1_CH1N, TIM1_CH2N seront nos 4 broches pour la PWM
- On a une fréquence de fonctionnement de 16 KHz = SysClk/(ARR+1)(PSC+1)=170 MHz/(1023+1)(9+1)
- On a un Death Time à 220 ce qui correspond à 2 us
- Comme on veut 60% de duty cycle, on met un pulse de 613 pour le ch1 et 2

Etat d'avancement scéance 1: PWMs, EXTI et code commande générés 
Etape en cours : gestion de la vitesse

