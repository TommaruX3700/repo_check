Note personali sul lavoro:
- La classe NotificationServer è inutilmente complessa, specie perchè presenta l'implementazione di multithreading per un processo semplice e veloce come l'append su file di log, eventuali librerie mqtt implementano autonomamente l'invio asincrono di messaggi;
- La suddivisione di due metodi "CslMsg" e "NotificationServer::send()" è confusionaria sul loro utilizzo;
- troppa codipendendeza tra NotificationServer e public.hpp