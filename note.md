## Note personali sul lavoro:

- La classe NotificationServer è inutilmente complessa, specie perchè presenta l'implementazione di multithreading per un processo semplice e veloce come l'append su file di log, eventuali librerie mqtt implementano autonomamente l'invio asincrono di messaggi. Tuttavia rimane utile come template se si decide di implementare invio di notifica tramite metodi che possono richiedere più tempo del previsto.

- La suddivisione di due metodi "CslMsg" e "NotificationServer::send()" è confusionaria sul loro utilizzo;

- troppa codipendendeza tra tutto il progetto e public.hpp