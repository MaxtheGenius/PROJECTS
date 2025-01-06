use anchor_lang::prelude::*;
use anchor_lang::solana_program::clock::Clock;
use solana_program::pubkey::Pubkey;
use rand::Rng;

#[cfg(test)]
mod tests {
    use super::*;
    use anchor_lang::prelude::program;
    use anchor_lang::solana_program::program::invoke_signed;
    use solana_program::instruction::{AccountMeta, Instruction};
    use solana_program::system_program;

    #[test]
    fn test_lend_and_borrow() {
        let user = Keypair::new();
        let pool = Keypair::new();
        let program_id = Pubkey::from_str("DeFi1234Defi1234DeFi1234DeFi1234DeFi1234").unwrap();

        let amount_to_lend = rand::thread_rng().gen_range(100..1000);
        let amount_to_borrow = rand::thread_rng().gen_range(100..500);

        let mut pool_balance = 0;

        // Simulate lending
        let lend_ix = Instruction {
            program_id,
            accounts: vec![
                AccountMeta::new(user.pubkey(), true),
                AccountMeta::new(pool.pubkey(), false),
            ],
            data: vec![1, 0, amount_to_lend as u8],  // Randomize data for lending
        };

        let lend_res = invoke_signed(&lend_ix, &[], &[]);

        assert!(lend_res.is_ok(), "Lend failed!");

        pool_balance += amount_to_lend;

        // Simulate borrowing
        let borrow_ix = Instruction {
            program_id,
            accounts: vec![
                AccountMeta::new(user.pubkey(), true),
                AccountMeta::new(pool.pubkey(), false),
            ],
            data: vec![2, 0, amount_to_borrow as u8],  // Randomize data for borrowing
        };

        let borrow_res = invoke_signed(&borrow_ix, &[], &[]);

        assert!(borrow_res.is_ok(), "Borrow failed!");

        pool_balance -= amount_to_borrow;
    }
}