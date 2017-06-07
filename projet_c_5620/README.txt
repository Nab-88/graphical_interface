                    ===============================
                    =                             =
                    =    INTERFACE GRAPHIQUE      =
                    =                             =
                    ===============================
                              Auteurs :
                              -> Louis Segretin
                              -> Luc Boussant-Roux
                              -> Nicolas Bernard
                              le 7 Juin 2017.

                    ===============================
                    =          Description         =
                    ===============================

Cette bibliothèque logicielle facilite la programmation des
Interfaces Utilisateur Graphiques (IUG). En utilisant cette bibliothèque,
 un programmeur pourra facilement créer une interface graphique composée de
fenêtres et d’interacteurs tels que boutons, fenêtres, etc.

                    ===============================
                    =          Prérequis          =
                    ===============================

La bibliothèque logicielle a besoin de la bibliothèque SDL pour fonctionner.

            ===================================================
            =         Organisation de la bibliothèque         =
            ===================================================

Dans le dossier /src :

-> les fichiers "draw":
  - ei_draw.c
  - ei_draw_poly.c
  - ei_draw_widgets.c

Ces fichiers permettent le dessin des "primitives graphiques", ce sont des
fonctions de bases que l'on utilisera pendant toute la suite du développement de
la bibliothèque. Exemple : créer un "polygone", le remplir, copier une surface,
écrire du texte, remplir une surface d'une couleur, dessiner un bouton, un toplevel.

-> les fichiers "widgets":
  - ei_widget.c
  - ei_widget_frame.c
  - ei_widget_button.c
  - ei_widget_toplevel.c

Ces fichiers gèrent la création, configuration et l'utilisation des 3
différentes classes de widgets : button, toplevel, frame.

-> le fichier ei_placer.c :
Le placeur gère la position et la taille des widgets présens à l'écran.

-> le fichier ei_event.c :
Ce fichier permet de paramétrer les widgets pour leur permettre d'appeler des
traitants internes et externes.

-> le fichier ei_application.c :
Dans ce fichier sont définies les fonctions qui permettent de lancer et de faire
fonctionner l'interface graphique

                    ===============================
                    =            Makefile         =
                    ===============================

make nom_du_test : permet de compiler le test en question
make all et make : compile tous les tests
make clean : supprimer tous les fichiers .o

                    ===============================
                    =            Tests            =
                    ===============================

-> button_modified.c
Ce test affiche un unique bouton cliquable qui contient du texte. Lors d'un click
sur le bouton, "Click" s'affiche dans le terminal.

-> frame_modified.c
Ce test permet de mettre en évidence le clipping d'une frame, d'une image dans un
frame, d'un toplevel, et le resizable d'un toplevel.

-> hello_world_modified.c
Ce test montre le bon fonctionnement de la gestion avant/arrière plan lorsque deux
toplevels sont superposées, resizable, le déplacement d'un toplevel, la bonne gestion
de l'affichage d'un bouton au sein d'une toplevel en arrière plan.

-> puzzle_modified.c
Ce test permet d'afficher un jeu de taquin. Ce test utilise un très grand nombre
des fonctionnalités de notre bibliothèque parmi lesquelles, l'affichage en transparence,
le dessin d'image, le clipping etc.

-> two048_modified.c
Implémentation du célèbre jeu 2048 grâce à notre bibliothèque.
Modification par rapport au test d'origine, on a placé la ligne 232 à la ligne 237.
Car selon nous ei_place n'était pas appelée au bon moment.
