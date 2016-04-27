Créer un programme qui fait appel à do_sched_setscheduler en c et le compiler normalement comme d'habitude.

Quand on le lance le noyau est donc censé afficher le message tout seul dans le terminal

Tutoriel pour pouvoir utiliser le noyau :
    -lancer kvm-term.sh dans le terminal
       -importer les fichiers avec scp -P 10022 mymodule.ko ase@127.0.0.1:./
       -ils se trouvent dans /home/ase


dans local/lepretre
    -extraire le tar.gz de linux 4.1.20
    -mettre l'image de debian32
    -mettre kvm32.config dans le dossier linux-4.1.20

dans le dossier linux 4.1.20:
    -make menuconfig
    -load kvm32.config
    -save kvm32.config
    -save .config
    -make -j8
