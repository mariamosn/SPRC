/*
 * Maria Moșneag
 * 343C1
 * Tema 1 SPRC
 */

#ifndef _TEMA1_DB_H
#define _TEMA1_DB_H

#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <iostream>

#define PERM_LEN    200

/*
 * key = user ID
 * value = current access token
 */
extern std::unordered_map<std::string, std::string> id_to_token;

/*
 * key = access token
 * value = token's current valability
 */
extern std::unordered_map<std::string, int> token_valab;

/*
 * key = req auth token
 * value = map
 * {
 *      key = resource
 *      value = vector,
 *              where v[i] = 1 if the user has permission for i
 *                           or 0 otherwise
 *                           and i is one of the following:
 *                           READ, INSERT, MODIFY, DELETE, EXECUTE
 * }
 */
extern std::unordered_map<std::string,
        std::unordered_map<std::string, std::vector<int>>> req_to_perm;

/*
 * key = access token
 * value = map
 * {
 *      key = resource
 *      value = vector,
 *              where v[i] = 1 if the user has permission for i
 *                           or 0 otherwise
 *                           and i is one of the following:
 *                           READ, INSERT, MODIFY, DELETE, EXECUTE
 * }
 */
extern std::unordered_map<std::string,
        std::unordered_map<std::string, std::vector<int>>> acc_to_perm;

/*
 * empty permissions map
 */
extern std::unordered_map<std::string, std::vector<int>> empty_perm;

/*
 * set with all the server's resources
 */
extern std::unordered_set<std::string> resources;

/*
 * vector with maps:
 * {
 *      key = resource
 *      value = vector,
 *              where v[i] = 1 if the user has permission for i
 *                           or 0 otherwise
 *                           and i is one of the following:
 *                           READ, INSERT, MODIFY, DELETE, EXECUTE
 * }
 */
extern std::vector<std::unordered_map<std::string, std::vector<int>>> approvals;

/*
 * next approval's index
 */
extern int next_approval;

/*
 * default token valability
 */
extern int default_val;

#endif /* !_TEMA1_DB_H */
