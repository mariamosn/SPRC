/*
 * Maria Moșneag
 * 343C1
 * Tema 1 SPRC
 */

#include "db.h"
#include <unordered_map>
#include <vector>
#include <unordered_set>

/*
 * key = user ID
 * value = current access token
 */
std::unordered_map<std::string, std::string> id_to_token;

/*
 * key = access token
 * value = token's current valability
 */
std::unordered_map<std::string, int> token_valab;

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
std::unordered_map<std::string,
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
std::unordered_map<std::string,
        std::unordered_map<std::string, std::vector<int>>> acc_to_perm;

/*
 * empty permissions map
 */
 std::unordered_map<std::string, std::vector<int>> empty_perm;

/*
 * set with all the server's resources
 */
 std::unordered_set<std::string> resources;

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
std::vector<std::unordered_map<std::string, std::vector<int>>> approvals;

/*
 * next approval's index
 */
int next_approval;

/*
 * default token valability
 */
int default_val;
