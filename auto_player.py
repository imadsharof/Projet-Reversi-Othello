'''
Auto-player permet de faire tourner une partie de reversi selon un scénario prédéfini.

Le programme prend en paramètre:
- le type de partie à jouer (H, HF, FH, HFH, AFA)
- le chemin vers le programme
- le chemin vers le deuxième programme (utile si type = HFH ou AFA, sinon il est ignoré)
- le chemin vers le dossier contenant les coups à jouer (utile si type = H, HF, FH ou HFH, sinon il est ignoré)
- le chemin vers le dossier où seront stockées les parties jouées (utile si type = HF, FH, HFH ou AFA, sinon il est ignoré)
- timeout: le temps limite pour chaque partie (en secondes)
l'ordre des paramètres est important et tous les paramètres doivent être présents, même si ils ne sont pas utilisés.
Si un paramètre n'est pas utilisé, il peut être remplacé par n'importe quelle valeur.

exemple d'utilisation:
- python3 auto_player.py H ./projet - scenario/ ./ 10
  -> lance une partie humain contre humain avec le programme ./projet, suivant les coups contenu dans les fichiers blanc.txt et noir.txt du dossier scenario/. timeout de 10 secondes
- python3 auto_player.py HFH ./projet ./projet2 scenario/ ./ 300 
  -> lance les programmes ./projet et ./projet2 pour jouer l'un contre l'autre, suivant les coups contenu dans le dossier scenario/. timeout de 5 minutes
- python3 auto_player.py AFA ./projet ./projet2 - ./ 300
  -> lance les programmes ./projet et ./projet2 pour jouer l'un contre l'autre, en utilisant leur IA respective. timeout de 5 minutes

Le type de partie est défini comme suit:
- H: Humain contre humain, un seul programme est lancé
- HF ou FH: Humain contre fichier, un seul programme est lancé, le fichier contenant les coups du deuxième joueur est complété avant de lancer la partie.
  HF: le joueur 1 est humain, FH: le joueur 2 est humain
- HFH: deux programmes sont lancés pour jouer l'un contre l'autre via fichier, l'un joue humain contre fichier, l'autre fichier contre humain
- AFA: deux programmes sont lancés pour jouer l'un contre l'autre via fichier, l'un joue AI contre fichier, l'autre fichier contre AI

Une fois la partie effectuée, les résultats sont sauvegardés dans des fichiers:
- p1_input.txt: les inputs envoyés au programme 1
- p1_output.txt: la sortie standard du programme 1
- p1_error.txt: la sortie d'erreur du programme 1
- p1_indicators.txt: les indicateurs de la partie (temps, scores, etc.)

si type = HFH ou AFA, les fichiers p2_input.txt, p2_output.txt, p2_error.txt et p2_indicators.txt sont aussi créés pour le programme 2.
'''

import os
import sys
import subprocess
from concurrent.futures import ThreadPoolExecutor, as_completed
import time
import re

def clean_game_folder(dossier_jeu):
    # Supprime les fichiers blanc.txt et noir.txt du dossier_jeu
    for filename in ['blanc.txt', 'noir.txt']:
        if os.path.exists(os.path.join(dossier_jeu, filename)):
            os.remove(os.path.join(dossier_jeu, filename))

def parse_scores(output):
    '''
    Parse la sortie standard du programme pour récupérer les scores des joueurs.
    les deux dernières lignes doivent contenir les scores des joueurs, sous la forme:
    Noir: scorenoir
    Blanc: scoreblanc

    '''

    lines = [l for l in output.split('\n') if l.strip() != '']

    if len(lines) < 2:
        return (False, False)
    
    # A l'aide d'une regex on vérifie que les deux dernières lignes sont bien les scores
    match_noir = re.match(r'[Nn]oir:\s*(\d+)', lines[-2])
    match_blanc = re.match(r'[Bb]lanc:\s*(\d+)', lines[-1])

    if not match_noir or not match_blanc:
        return (False, False)
    else:
        return (int(match_noir.group(1)), int(match_blanc.group(1)))
    
def prepare_input(joueur1, joueur2, dossier_scenario, dossier_jeu):
    '''
    parametres:
    - joueur1: le type du joueur 1 (H, F ou A)
    - joueur2: le type du joueur 2 (H, F ou A)
    - dossier_scenario: le nom du dossier contenant les fichiers de scenario (les coups à jouer)
    - dossier_jeu: le nom du dossier où seront stockées les parties jouées, dans le cas des joueurs F

    retourne:
    - une liste de lignes à envoyer au programme
    '''

    inputs = []

    # Choix du premier joueur
    if joueur1 == 'H':
        inputs.append('H')
        inputs.append('JoueurNoir')
    elif joueur1 == 'F':
        inputs.append('F')
        inputs.append(dossier_jeu)
    elif joueur1 == 'A':
        inputs.append('A')

    # Choix du deuxième joueur
    if joueur2 == 'H':
        inputs.append('H')
        inputs.append('JoueurBlanc')
    elif joueur2 == 'F':
        inputs.append('F')
        inputs.append(dossier_jeu)
    elif joueur2 == 'A':
        inputs.append('A')
    
    # Ajout de "enter" pour démarrer la partie
    inputs.append('')
    
    # Si pas de joueur humain, on doit juste valider chaque coup en envoyant 'enter'
    # On ne connait pas à l'avance le nombre de coup, mais on sait qu'il y a moins de 60 coups par joueur
    # On envoie donc 120 fois 'enter'
    if joueur1 != 'H' and joueur2 != 'H':
        for i in range(120):
            inputs.append('')
    else:
        # On récupère les coups du scénario dans les fichier noir.txt (pour le joueur 1) et blanc.txt (pour le joueur 2)
        # On suppose que les fichiers sont bien formés (un coup par ligne)

        # On récupère les coups du joueur 1
        fichier_joueur_1 = os.path.join(dossier_scenario, 'noir.txt')
        with open(fichier_joueur_1, 'r') as f:
            coups_joueur_1 = f.readlines()
        
        # On récupère les coups du joueur 2
        fichier_joueur_2 = os.path.join(dossier_scenario, 'blanc.txt')
        with open(fichier_joueur_2, 'r') as f:
            coups_joueur_2 = f.readlines()
        
        n_coups = max(len(coups_joueur_1), len(coups_joueur_2))

        # On ajoute les coups à jouer dans la liste des inputs
        for i in range(n_coups):
            
            if joueur1 == 'H':
                inputs.append(coups_joueur_1[i].strip() if i < len(coups_joueur_1) else '')
            inputs.append('')

            if joueur2 == 'H':
                inputs.append(coups_joueur_2[i].strip() if i < len(coups_joueur_2) else '')
            inputs.append('')
    
    with open('inputs.txt', 'w') as f:
        f.write('\n'.join(inputs))
    return inputs
        


def run_game(programme, joueur1, joueur2, dossier_scenario, dossier_jeu, timeout, wrong_input=False):
    '''
    parametres:
    - programme: le nom du programme qui va executer la partie. ex: './projet'
    - joueur1: le type du joueur 1 (H, F ou A)
    - joueur2: le type du joueur 2 (H, F ou A)
    - dossier_scenario: le nom du dossier contenant les fichiers de scenario (les coups à jouer)
    - dossier_jeu: le nom du dossier où seront stockées les parties jouées, dans le cas des joueurs F

    return:
    - stdout: la sortie standard du programme
    - stderr: la sortie d'erreur du programme
    - time: le temps d'exécution du programme
    - timed_out: True si le programme a dépassé le temps limite, False sinon
    - statut: le code de retour du programme
    - scores: les scores des joueurs (noir, blanc)
    '''

    # On prépare les inputs à envoyer au programme
    inputs = prepare_input(joueur1, joueur2, dossier_scenario, dossier_jeu)

    if wrong_input == 'no_confirmations':
        # On enlève les confirmations de coups validés
        inputs = [i for i in inputs if i != '']

    # On lance le programme
    process = subprocess.Popen(programme, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

    try:
        start_time = time.time()
        stdout, stderr = process.communicate('\n'.join(inputs), timeout=timeout)
        duration = time.time() - start_time
        timed_out = False
    except subprocess.TimeoutExpired:
        process.kill()
        stdout, stderr = process.communicate()
        duration = timeout
        timed_out = True
    # Gestion d'interruption par l'utilisateur
    except KeyboardInterrupt as e:
        process.kill()
        stdout, stderr = process.communicate()
        stderr += f'\nProcess terminated by user \n {e}'
        duration = time.time() - start_time
        timed_out = False
    # Cas d'erreur inconnue
    except Exception as e:
        process.kill()
        stdout, stderr = process.communicate()
        stderr += f'\nProcess terminated with exception \n {e}'
        duration = time.time() - start_time
        timed_out = False

    return {
        'inputs': inputs,
        'stdout': stdout,
        'stderr': stderr,
        'time': duration,
        'timed_out': timed_out,
        'statut': process.returncode,
        'scores': parse_scores(stdout)
    }

def program_match(programme1, programme2, dossier_jeu, AI=True, scenario=None, timeout=300):
    '''
    Fait jouer les programme programme1 et programme2 l'un contre l'autre, en utilisant la communication par fichier à travers le dossier dossier_jeu.
    Si AI est True, les joueurs sont des IA, sinon, il faut utiliser des coups prédéfinis dans le dossier scenario.
    '''

    player_type = 'A' if AI else 'H'

    args_programme1 = [programme1, player_type, 'F', scenario, dossier_jeu, timeout]
    args_programme2 = [programme2, 'F', player_type, scenario, dossier_jeu, timeout]
    try:
        with ThreadPoolExecutor() as executor:
            # Submit the subprocess tasks
            future1 = executor.submit(run_game, *args_programme1)
            future2 = executor.submit(run_game, *args_programme2)

            # Wait for both subprocesses to complete
            results = list(as_completed([future1, future2]))
    except Exception as e:
        print(e)
        results =[{
            'inputs': [],
            'stdout': "",
            'stderr': f"Process terminated with exception: {e}",
            'time': False,
            'timed_out': None,
            'statut': -1,
            'scores': (False, False)
        }, None]
    
    return results[0].result(), results[1].result()

def run_against_file(programme, joueur_2_est_fichier, dossier_scenario, dossier_jeu, timeout):
    '''
    Lance le programme en mode HvF ou FvH selon la valeur de joueur_2_est_fichier (qui doit être boolean).
    Le fichier du joueur fichier est placé déjà complet dans le dossier_jeu. De cette manière, 
    on peut tester la fonctionnalité fichier du programme sans devoir lancer 2 programmes en parallèle.
    '''

    if joueur_2_est_fichier:
        joueur1 = 'H'
        joueur2 = 'F'
        filename = 'blanc.txt'
        playerName = 'JoueurBlanc'
    else:
        joueur1 = 'F'
        joueur2 = 'H'
        filename = 'noir.txt'
        playerName = 'JoueurNoir'

    # On copie le fichier scenario du joueur dans le dossier_jeu, en ajoutant le nom du joueur en première ligne
    with open(os.path.join(dossier_jeu, filename), 'w') as f:
        f.write(f'{playerName}\n')
        with open(os.path.join(dossier_scenario, filename), 'r') as f2:
            f.write(f2.read())
    
    # On lance le programme
    return run_game(programme, joueur1, joueur2, dossier_scenario, dossier_jeu, timeout)


def save_results(results, program_name):
    '''
    Sauvegarde les résultats dans une série de fichiers.
    Un pour l'input, un pour l'ouput, un pour l'erreur, un pour les autres indicateurs (temps, scores, etc.)
    '''

    with open(f'{program_name}_input.txt', 'w') as f:
        f.write('\n'.join(results['inputs']))

    with open(f'{program_name}_output.txt', 'w') as f:
        f.write(results['stdout'])
    
    with open(f'{program_name}_error.txt', 'w') as f:
        f.write(results['stderr'])
    
    with open(f'{program_name}_indicators.txt', 'w') as f:
        f.write(f'Time: {results["time"]}\n')
        f.write(f'Scores: {results["scores"]}\n')
        f.write(f'Timed out: {results["timed_out"]}\n')
        f.write(f'Statut: {results["statut"]}\n')

def autoplayer(type_de_jeu, programme1, programme2, dossier_scenario, dossier_jeu, timeout):
    '''
    Lance une partie selon le type de jeu spécifié.
    Retourne les résultats de la partie.
    '''

    if dossier_jeu:
        clean_game_folder(dossier_jeu)

    r1, r2 = None, None
    equivalent_command = f'python3 auto_player.py {type_de_jeu} {programme1} {programme2} {dossier_scenario} {dossier_jeu} {timeout}'

    try:

        if type_de_jeu == 'H':
            r1 = run_game(programme1, 'H', 'H', dossier_scenario, dossier_jeu=None, timeout=timeout)
        elif type_de_jeu == 'HF' or type_de_jeu == 'FH':
            r1 = run_against_file(programme1, type_de_jeu == 'HF', dossier_scenario, dossier_jeu, timeout)
        elif type_de_jeu == 'HFH':
            r1, r2 = program_match(programme1, programme2, dossier_jeu, AI=False, scenario=dossier_scenario, timeout=timeout)
        elif type_de_jeu == 'AFA':
            r1, r2 = program_match(programme1, programme2, dossier_jeu, AI=True, scenario=None, timeout=timeout)
        elif type_de_jeu == 'no_confirmations':
            r1 = run_game(programme1, 'H', 'H', dossier_scenario, dossier_jeu=None, timeout=timeout, wrong_input='no_confirmations')
        
    except Exception as e:
        r1 =  {
            'inputs': [],
            'stdout': "",
            'stderr': f"Process terminated with exception: {e}",
            'time': False,
            'timed_out': None,
            'statut': -1,
            'scores': (False, False)
        }
    except KeyboardInterrupt:
        r1 =  {
            'inputs': [],
            'stdout': "",
            'stderr': f"Process terminated by the user",
            'time': False,
            'timed_out': None,
            'statut': -1,
            'scores': (False, False)
        }
        
    
    return r1, r2, equivalent_command


def from_command_line():
    '''
    Permet d'utiliser la fonction autoplayer à partir de la ligne de commande.
    On récupère les paramètres: programme, joueur1, joueur2, dossier_scenario, dossier_jeu dans les arguments de la ligne de commande
    On suppose que les paramètres sont bien formés

    Les résultats sont sauvegardés dans des fichiers.
    '''

    type_de_jeu = sys.argv[1] # 'H', 'HF', 'FH', 'HFH' ou 'AFA'
    
    programme1 = sys.argv[2]
    programme2 = sys.argv[3]
    dossier_scenario = sys.argv[4]
    dossier_jeu = sys.argv[5]
    timeout = int(sys.argv[6])

    r1, r2, _ = autoplayer(type_de_jeu, programme1, programme2, dossier_scenario, dossier_jeu, timeout)

    save_results(r1, 'p1')
    if r2:
        save_results(r2, 'p2')

    

if __name__ == '__main__':
    from_command_line()


