#include <CompuGuard.h>

void GenerateUUID(LPTSTR szUUID) {
	UUID bUuid;
	RPC_CSTR rstrUUID;

	UuidCreate(&bUuid);
	UuidToString(&bUuid, &rstrUUID);
	lstrcpy(szUUID, rstrUUID);
	RpcStringFree(&rstrUUID);
}
