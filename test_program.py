'''
Lance une série de tests sur un programme de reversi

Usage:
python test_program.py <programme> [<fichier de sortie> [<programme2>]]

- programme: chemin vers le programme à tester
- fichier de sortie: fichier dans lequel écrire une synthèse des résultats des tests. Si non spécifié, les résultats sont uniquement affichés dans la console
- programme2: chemin vers le programme à utiliser comme adversaire pour les tests faisant s'affronter deux programmes. Si non spécifié, le programme est lancé contre lui-même
'''

import auto_player
import os
import sys

SINGLE_PROGRAM_TIMEOUT = 5
DOUBLE_PROGRAM_TIMEOUT = 300
MAIN_SCENARIOS_FOLDER = 'matchfiles/'
SCENARIOS_H = [
    {'match': 'match-7-10', 'correct_scores': [55, 9]},
    {'match': 'match-7-19', 'correct_scores': [61, 3]},
    {'match': 'match-10-13', 'correct_scores': [51, 13]},
    {'match': 'match-13-19', 'correct_scores': [27, 36]},
    {'match': 'match-19-10', 'correct_scores': [29, 35]},
]

SCENARIOS_HF = [
    {'match': 'match-7-10', 'correct_scores': [55, 9]},
    {'match': 'match-7-19', 'correct_scores': [61, 3]},
    {'match': 'match-10-13-wrong-inputs', 'correct_scores': [51, 13]},
    {'match': 'match-13-19', 'correct_scores': [27, 36]},
    {'match': 'match-19-10', 'correct_scores': [29, 35]},
]

def shorten_output(output, max_lines=10, max_length=150):
    '''
    Retourne les max_lines dernières lignes de la sortie d'un programme, en limitant à max_length caractères par ligne
    '''

    output_lines = output.split('\n')
    last_lines = output_lines[-max_lines:]
    shortened_lines = []
    for line in last_lines:
        shortened_lines.append(line[:max_length])
    return '\n'.join(shortened_lines)

def clean_game_folder(dossier_jeu):
    # Supprime les fichiers blanc.txt et noir.txt du dossier_jeu
    for filename in ['blanc.txt', 'noir.txt']:
        if os.path.exists(os.path.join(dossier_jeu, filename)):
            os.remove(os.path.join(dossier_jeu, filename))

def testHvH(program, scenario, correct_scores):
    '''
    Teste un programme en mode humain contre humain, selon les coups donnés dans le dossier scénario
    '''
    results, _, command = auto_player.autoplayer('H', programme1=program, programme2=None, dossier_scenario=scenario, dossier_jeu='./', timeout=SINGLE_PROGRAM_TIMEOUT)

    error_message = []
    test_succeeded = True

    if results['statut'] != 0:
        error_message.append('Le programme a échoué')
        test_succeeded = False
    else:
        if results['scores'][0] != correct_scores[0] or results['scores'][1] != correct_scores[1]:
            error_message.append(f'Les scores sont incorrects. Les scores corrects sont Noir: {correct_scores[0]} Blanc: {correct_scores[1]}. Les scores calculés par votre programme sont Noir: {results["scores"][0]} Blanc: {results["scores"][1]}')
            test_succeeded = False
    
    if not test_succeeded:
        error_message.append(f'Relancez ce test-ci avec la commande suivante:\n{command}')

    return test_succeeded, error_message, results


def testHvF(program, scenario, correct_scores):
    '''
    Teste un programme en mode humain contre fichier, selon les coups donnés dans le dossier scénario
    le fichier est créé complet dans le dossier jeu dès le début de la partie, 
    il n'est pas mis à jour au fur et à mesure de la partie comme en condition réelle
    '''

    error_message = []
    test_succeeded = True

    # Test dans le sens humain contre fichier
    results, _, command1 = auto_player.autoplayer('HF', programme1=program, programme2=None, dossier_scenario=scenario, dossier_jeu='./', timeout=SINGLE_PROGRAM_TIMEOUT)

    if results['statut'] != 0:
        error_message.append('Le programme a échoué dans le sens humain contre fichier')
        test_succeeded = False
    else:
        if results['scores'][0] != correct_scores[0] or results['scores'][1] != correct_scores[1]:
            error_message.append(f'Les scores sont incorrects. Les scores corrects sont Noir: {correct_scores[0]} Blanc: {correct_scores[1]}. Les scores calculés par votre programme sont Noir: {results["scores"][0]} Blanc: {results["scores"][1]}')
            test_succeeded = False

    # Test dans le sens fichier contre humain
    results, _, command2 = auto_player.autoplayer('FH', programme1=program, programme2=None, dossier_scenario=scenario, dossier_jeu='./', timeout=SINGLE_PROGRAM_TIMEOUT)
    if results['statut'] != 0:
        error_message.append('Le programme a échoué dans le sens fichier contre humain')
        test_succeeded = False
    else:
        if results['scores'][0] != correct_scores[0] or results['scores'][1] != correct_scores[1]:
            error_message.append(f'Les scores sont incorrects. Les scores corrects sont Noir: {correct_scores[0]} Blanc: {correct_scores[1]}. Les scores calculés par votre programme sont Noir: {results["scores"][0]} Blanc: {results["scores"][1]}')
            test_succeeded = False
    
    if not test_succeeded:
        error_message.append(f'Relancez ce test-ci avec les commandes suivantes:\n{command1}\n{command2}')

    return test_succeeded, error_message, results


def testFvF(program1, program2, scenario, correct_scores):
    '''
    Teste deux programmes en mode humain vs fichier, les deux programmes jouant l'un contre l'autre par fichier interposé
    '''

    dossier_jeu = './'
    
    #clean_game_folder(dossier_jeu)

    r1, r2, command = auto_player.autoplayer('HFH', programme1=program1, programme2=program2, dossier_scenario=scenario, dossier_jeu='./', timeout=DOUBLE_PROGRAM_TIMEOUT)
    
    error_message = []
    test_succeeded = True

    if r1['statut'] != 0 or r2['statut'] != 0:
        error_message.append('Le programme a échoué')
        test_succeeded = False
    else:
        if r1['scores'][0] != correct_scores[0] or r1['scores'][1] != correct_scores[1]:
            error_message.append(f'Les scores sont incorrects. Les scores corrects sont Noir: {correct_scores[0]} Blanc: {correct_scores[1]}. Les scores calculés par votre programme sont Noir: {r1["scores"][0]} Blanc: {r1["scores"][1]}')
            test_succeeded = False
        elif r2['scores'][0] != correct_scores[0] or r2['scores'][1] != correct_scores[1]:
            error_message.append(f'Les scores sont incorrects. Les scores corrects sont Noir: {correct_scores[0]} Blanc: {correct_scores[1]}. Les scores calculés par votre programme sont Noir: {r2["scores"][0]} Blanc: {r2["scores"][1]}')
            test_succeeded = False

    if not test_succeeded:
        error_message.append(f'Relancez ce test-ci avec la commande suivante:\n{command}')

    return test_succeeded, error_message, r1

def testAvA(program1, program2):
    '''
    Teste deux programmes en mode AI vs fichier, les deux programmes jouant l'un contre l'autre par fichier interposé
    '''

    dossier_jeu = './'
    
    #clean_game_folder(dossier_jeu)
    
    r1, r2, command = auto_player.autoplayer('AFA', programme1=program1, programme2=program2, dossier_scenario=None, dossier_jeu=dossier_jeu, timeout=DOUBLE_PROGRAM_TIMEOUT)
    
    error_message = []
    test_succeeded = True

    if r1['statut'] != 0 or r2['statut'] != 0:
        error_message.append('Le programme a échoué')
        test_succeeded = False
    else:
        if r1['scores'][0] != r2['scores'][0] or r1['scores'][1] != r2['scores'][1]:
            error_message.append(f'Les 2 programmes n\'arrivent pas au même score. L\'un arrive à Noir: {r1["scores"][0]} Blanc: {r1["scores"][1]}. L\'autre à Noir: {r2["scores"][0]} Blanc: {r2["scores"][1]}')
            test_succeeded = False

    if not test_succeeded:
        error_message.append(f'Relancez ce test-ci avec la commande suivante:\n{command}')
    
    return test_succeeded, error_message, r1

def testBadInputs(program, scenario):
    '''
    Teste un programme en mode humain contre humain avec de mauvais inputs: pas de confirmations des coups.

    Le programme doit échouer pour que le test réussisse
    '''
    results, _, command = auto_player.autoplayer('no_confirmations', programme1=program, programme2=None, dossier_scenario=scenario, dossier_jeu='./', timeout=SINGLE_PROGRAM_TIMEOUT)

    error_message = []
    test_succeeded = True

    if results['statut'] == 0:
        error_message.append('Le programme s\'est terminé sans erreur alors que les inputs étaient incorrects')
        error_message.append('Votre programme devrait demander une confirmation avant de jouer un coup, il semble qu\'il ne le fasse pas')
        test_succeeded = False
    
    if not test_succeeded:
        error_message.append(f'Relancez ce test-ci avec la commande suivante:\n{command}')

    return test_succeeded, error_message, results


def run_tests_suite(test_function, scenarios_tests):
    '''
    Lance une séries de scenarios de tests sur une fonction de test et synthétise les résultats
    '''

    success = 0
    failure = 0
    timed_out = 0
    time = []
    all_error_messages = []

    failed_output = ''
    failed_error = ''

    for i, scenario in enumerate(scenarios_tests):
        print(f'Test {i+1}/{len(scenarios_tests)}:...', end='')
        test_succeeded, error_messages, results = test_function(scenario)
        time.append(results['time'])
        if test_succeeded:
            success += 1
            print('Réussi')
        else:
            print('Échoué')
            failure += 1
            if results['timed_out']:
                timed_out += 1
            all_error_messages.append(f"Test n°{i+1}")
            all_error_messages += error_messages

            failed_output = results['stdout']
            failed_error = results['stderr']
    
    metrics = {
        'success': success,
        'failure': failure,
        'timed_out': timed_out,
        'average_time': sum(time) / len(time),
    }

    print(f'Nombre de tests réussis: {success}')
    print(f'Nombre de tests échoués: {failure}')
    print(f'Nombre de tests ayant dépassé le temps limite: {timed_out}')
    print(f'Temps moyen: {metrics["average_time"]}')
    
    for message in all_error_messages:
        print(message)
    
    if failure > 0 and failed_output != '':
        print('stdout du dernier match qui a échoué:')
        print(shorten_output(failed_output))
    if failure > 0 and failed_error != '':
        print('stderr du dernier match qui a échoué:')
        print(shorten_output(failed_error))
    
    print('')
    
    return metrics

def run_FvF_tests_suite(program1, program2):
    '''
    Teste deux programmes en mode humain contre fichier, les deux programmes jouant l'un contre l'autre par fichier interposé
    '''

    print(f'Tests de {program1} contre {program2} suivant des scénarios prédéfinis')

    def test_function(scenario):
        scenario_folder = MAIN_SCENARIOS_FOLDER + scenario['match'] + '/'
        return testFvF(program1, program2, scenario_folder, scenario['correct_scores'])
    
    return run_tests_suite(test_function, SCENARIOS_HF[:3])

def run_HvH_tests_suite(program):
    '''
    Teste le programme en mode HvH sur plusieurs scénarios et fait une synthèse des résultats
    '''

    print(f'Tests en mode humain contre humain')

    def test_function(scenario):
        scenario_folder = MAIN_SCENARIOS_FOLDER + scenario['match'] + '/'
        return testHvH(program, scenario_folder, scenario['correct_scores'])
    
    return run_tests_suite(test_function, SCENARIOS_H)

def run_HvF_tests_suite(program):
    '''
    Teste le programme en mode HvF sur plusieurs scénarios et fait une synthèse des résultats
    '''

    print(f'Tests en mode humain contre fichier')

    def test_function(scenario):
        scenario_folder = MAIN_SCENARIOS_FOLDER + scenario['match'] + '/'
        return testHvF(program, scenario_folder, scenario['correct_scores'])
    
    return run_tests_suite(test_function, SCENARIOS_HF)

def run_AvA_tests_suite(program1, program2):
    '''
    Teste deux programmes en mode AI contre fichier, les deux programmes jouant l'un contre l'autre par fichier interposé
    '''

    print(f'Tests de l\'AI de {program1} contre l\'AI de {program2}')

    def test_function(scenario):
        return testAvA(program1, program2)
    
    return run_tests_suite(test_function, range(3))

def run_bad_inputs_tests_suite(program):
    '''
    Vérifie que le programme nécessite bien les bons inputs en le lancant avec des inputs incorrects
    '''

    print(f'Tests de {program} avec des inputs incorrects')
    test_succeeded, error_message, results = testBadInputs(program, MAIN_SCENARIOS_FOLDER + SCENARIOS_H[0]['match'])
    if test_succeeded:
        print('Le programme a échoué comme attendu')
    else:
        print('\n'.join(error_message))
        if results['stdout'] != '':
            print('\nstdout du match qui a échoué:')
            print(shorten_output(results['stdout']))
        if results['stderr'] != '':
            print('\nstderr du match qui a échoué:')
            print(shorten_output(results['stderr']))
    
    print('')
    return test_succeeded


def main():
    '''
    Lance les tests
    '''
    
    program = sys.argv[1]
    
    if len(sys.argv) > 2:
        metrics_file = sys.argv[2]
    else:
        metrics_file = None

    if len(sys.argv) > 3:
        program2 = sys.argv[2]
    else:
        program2 = program

    

    HvH_metrics = run_HvH_tests_suite(program)
    wrong_inputs_test_succeeded = run_bad_inputs_tests_suite(program)
    HvF_metrics = run_HvF_tests_suite(program)
    FvF_metrics = run_FvF_tests_suite(program, program2)
    AvA_metrics = run_AvA_tests_suite(program, program2)


    #clean_game_folder('./')


    if metrics_file is not None:
        metrics = {
            'HvH_success': f"{HvH_metrics['success']}/{HvF_metrics['success'] + HvH_metrics['failure']}",
            'HvH_average_time': HvH_metrics['average_time'],
            'HvH_timed_out': HvH_metrics['timed_out'],
            'HvF_success': f"{HvF_metrics['success']}/{HvF_metrics['success'] + HvF_metrics['failure']}",
            'HvF_average_time': HvF_metrics['average_time'],
            'HvF_timed_out': HvF_metrics['timed_out'],
            'FvF_success': f"{FvF_metrics['success']}/{FvF_metrics['success'] + FvF_metrics['failure']}",
            'FvF_average_time': FvF_metrics['average_time'],
            'FvF_timed_out': FvF_metrics['timed_out'],
            'AvA_success': f"{AvA_metrics['success']}/{AvA_metrics['success'] + AvA_metrics['failure']}",
            'AvA_average_time': AvA_metrics['average_time'],
            'AvA_timed_out': AvA_metrics['timed_out'],
            'wrong_inputs_test_succeeded': wrong_inputs_test_succeeded,
        }
        with open(metrics_file, 'w') as f:
            keys = list(metrics.keys())
            values = list(metrics.values())
            f.write(','.join(keys) + '\n')
            f.write(','.join([str(v) for v in values]) + '\n')


if __name__ == '__main__':
    main()
