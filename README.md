# ESTETIMAGE

***Librairies à installer***
- gphoto2 (libgphoto2-dev)
- sqlite3
- libjpeg
- OpenCV
- Télécharger Eigen et placer le dossier à la racine ~/
- Télécharger Sqliteman pour la gestion de base de données



***La base de données***

Vérifier que le fichier database_images est présent dans estetimage/db

S'il n'est pas là vous devez créer la base de données Sqliteman:
- Créer la table photo_param et la remplir (vous pouvez trouver le script SQL et les inscructions dans estetimage/db/createdbsql.cpp
- Enregistrer le fichier sqlite3 dans estetimage/db sous le nom database_images


***Votre appareil photo***

Allez dans estetimage/src/include/params.h 
Si votre appareil possède plus de paramètres qu'indiqués dans les tableaux aperture_tab, iso_tab, iso_tab_convert, shutterspeed_tab et shutterspeed_tab_convert, vous pouvez les rajouter; ou enlever les paramètres non pris en charge par votre appareil.


***Ajouter des photos pour agrandir la base de données***

nb: vos photos doivent avoir des données exif, au minimum l'ouverture, la vitesse d'obturation et les ISO.
Placez vos photos dans estetimage/db/images

- cd estetimage/analyse
- make clean
- make
./analyse ../db/images/*

--> Cette opération peut prendre du temps, surtout si vos photos sont lourdes.
Amélioration possible: réduire la taille des images avant l'analyse

***Créer la matrice de données***

Pour que vos nouvelles photos soient prises en compte, il faut extraire la matrice correspondante à la nouvelle base de données. Pour cela:

- Effacer estetimage/pca/data.mat
- cd estetimage/pca
- make clean
- make
- ./pca

--> si rien ne ce passe vérifiez dans main.cpp que la fonction fillTheMatrix n'est pas commentée

Ouvrir data.mat, copier toutes les lignes.
Ouvrir build-estetimage/data.mat et remplacer la matrice existante par la nouvelle (supprimer toutes les lignes après la 7ème et coller notre nouvelle matrice)
L'en-tête: " lignes :photos
colonnes : nbCont, isPort, nbPers, icolor, h,s,v, vh, vs, vv, r,g,b, vr, vg, vb                               row 62 col 16 
" est importante, il faut remplacer le nombre de lignes (row) par le nouveau nombre. Si vous n'avez pas ajouté d'autres paramètres d'analyse dans la matrice le nombre de colonnes reste à 16.


***************************************
***Utiliser l'application Estetimage***
***************************************
Mettre votre appareil photo en mode manuel, vous avez le choix de laisser l'auto-focus ou non. Si vous le retirer c'est à vous de faire votre point.

- cd build-estetimage
- cmake ../estetimage
- make -j
- ./estetimage
