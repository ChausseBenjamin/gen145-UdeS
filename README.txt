Pour compiler les fichiers de la problématique dans Geany:

  1. Ouvrir les fichiers de la problématique dans Geany via le menu "Fichier > Ouvrir...", puis en sélectionnant les 3 fichiers du projet, soit :
     - gestion_images.c
     - bibliotheque_images.c
     - bibliotheque_image.h

  2. Ouvrir le menu déroulant "Construire > Définir les commandes de construction". Ceci ouvre un dialogue.

  3. Dans ce dialogue, changer l'instruction (Commande) associée à "Construire" ("Build", en anglais) pour la suivante:
       gcc -Wall -o gestion_images gestion_images.c bibliotheque_images.c

  4. Cliquer sur le bouton "Valider" (dans la bas du dialogue)

  5. Ensuite, pour procéder à la construction du projet, utiliser l'une des 3 options suivantes :
     - Ouvrir le menu déroulant "Construire > Build"
     - Ou appuyer sur F9
     - Ou dans la barre d'outils cliquer sur l'icône "Build" (ressemblant à une brique rouge)
     
    *Le fichier exécutable qui est généré se nomme "gestion_images.exe" 

  6. Finalement, exécuter le projet comme suit :
     - Ouvrir le menu déroulant "Construire > Execute
     - Ou appuyer sur F5
     - Ou dans la barre d'outils cliquer sur l'icône "Execute" (comprenant des roues dentellées)

    *La console s'ouvrira en affichant les "printf" du code.