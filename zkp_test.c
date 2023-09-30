#include <openssl/zkp_gadget.h>
#include <openssl/bulletproofs.h>

int main(int argc, char *argv[]){
	if (argc <= 1) {
        printf("Invalid parameter!\n");
        return -1;
    }

	//input 
    int64_t plaintext = atoi(argv[1]);

    if (plaintext == 0) {
        printf("no");
        return 0;
    }
    
    int ret = 0;
    int left_bound_bits = 0, right_bound_bits = 16;
    //public parameters
    int curve_id = NID_X9_62_prime256v1;
    ZKP_TRANSCRIPT *transcript = NULL;
    ZKP_RANGE_PUB_PARAM *pp = NULL;
    EC_KEY *key = NULL;
    
    //setup pp
    if (!(transcript = ZKP_TRANSCRIPT_new(ZKP_TRANSCRIPT_METHOD_sha256(), "test")))
        goto err;

    if (!(key = EC_KEY_new_by_curve_name(curve_id)))
        goto err;

    if (!(EC_KEY_generate_key(key)))
        goto err;
    
    if (!(pp = ZKP_RANGE_PUB_PARAM_new(EC_KEY_get0_group(key), 32)))
        goto err;
        
    // P parameters
    BN_CTX *bn_ctx = NULL;
    BIGNUM *v;
    ZKP_RANGE_WITNESS *witness_p = NULL;
    ZKP_RANGE_CTX *ctx_p = NULL;
    ZKP_RANGE_PROOF *proof_p = NULL;
    
    //V parameters
    ZKP_RANGE_WITNESS *witness_v = NULL;
    ZKP_RANGE_CTX *ctx_v = NULL;
    
    //P, send witness_p, proof_p to V
    bn_ctx = BN_CTX_new();
    if (bn_ctx == NULL)
        goto err;

    v = BN_CTX_get(bn_ctx);
    if (v == NULL)
        goto err;

    BN_set_word(v, plaintext);

    if (!(witness_p = ZKP_RANGE_WITNESS_new(pp, NULL, v)))
        goto err;

    if (!(ctx_p = ZKP_RANGE_CTX_new(transcript, pp, witness_p, key)))
        goto err;

    if (!(proof_p = ZKP_RANGE_PROOF_prove(ctx_p, left_bound_bits, right_bound_bits)))
        goto err;
        
    //V, receive witness_p, proof_p from V, verify proof_p
	
	witness_v = witness_p;
	if (!(ctx_v = ZKP_RANGE_CTX_new(transcript, pp, witness_v, key)))
        goto err;
	if (!(ZKP_RANGE_PROOF_verify(ctx_v, proof_p, left_bound_bits, right_bound_bits)))
        goto err;
		
	ret = 1;
	
err:
    ZKP_RANGE_PROOF_free(proof_p);
    ZKP_RANGE_CTX_free(ctx_p);
    ZKP_RANGE_WITNESS_free(witness_v);
    ZKP_RANGE_WITNESS_free(witness_p);
    ZKP_RANGE_PUB_PARAM_free(pp);
    EC_KEY_free(key);
    BN_CTX_free(bn_ctx);
	
	printf("%ld ",plaintext);
    printf("%s range: [0, 65535]\n", ret == 1 ? "in" : "not in");
}
