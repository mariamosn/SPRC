/*
 * Maria Moșneag
 * 343C1
 * Tema 1 SPRC
 */

/*
 * request_t
 * - este folosit pentru a stoca informația ce este trimisă procedurilor din
 *   cadrul server-ului
 * - nu este necesar ca toate câmpurile să fie folosite simultan
 * - id 		= id-ul user-ului
 * - type 		= codificarea operației
 * - token 		= token-ul necesar în cadrul operației
 * - additional = resursa sau tipul de request (cu sau fără reînnoire automată)
 * - cmd 		= comanda primită de la utilizator
 */
struct request_t {
	string id<>;
	int type;
	string token<>;
	string additional<>;
	string cmd<>;
};


/*
 * response_t
 * - este folosit pentru a stoca informația primită de la procedurile din cadrul
 *   server-ului
 * - nu este necesar ca toate câmpurile să fie folosite simultan
 * - result		= eroarea apărută
 * - req_token	= token-ul de autorizare
 * - acc_token	= token-ul de acces
 * - ref_token	= token-ul de refresh
 * - valability	= valabilitatea rămasă a token-ului de acces
 */
struct response_t {
	string result<>;
	string req_token<>;
	string acc_token<>;
	string ref_token<>;
	int valability;
};

program TEMA1_PROG {
	version TEMA1_VERS {
		/*
		* request_authorization
		* - generează token-ul de autorizare
		*/
		response_t request_authorization(request_t) = 1;

		/*
		* request_access_token
		* - generează token-ul de acces
		*/
		response_t request_access_token(request_t) = 2;

		/*
		* validate_delegated_action
		* - verifică dacă operația dorită este posibilă
		*/
		response_t validate_delegated_action(request_t) = 3;

		/*
		* approve_request_token
		* - validează token-ul de autorizare și generează permisiunile
		*/
		response_t approve_request_token(request_t) = 4;
	} = 1;
} = 1;
