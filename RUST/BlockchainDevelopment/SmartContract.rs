use anchor_lang::prelude::*;
use solana_program::program_error::ProgramError;
use rand::Rng;  // Random data generation

declare_id!("DeFi1234Defi1234DeFi1234DeFi1234DeFi1234");

#[program]
pub mod solana_defi {
    use super::*;

    // Define a structure to hold the pool state
    pub fn initialize_pool(ctx: Context<InitializePool>, amount: u64) -> Result<()> {
        let pool = &mut ctx.accounts.pool;
        pool.balance = amount;
        Ok(())
    }

    // Lending function
    pub fn lend(ctx: Context<Lend>, amount: u64) -> Result<()> {
        let user = &mut ctx.accounts.user;
        let pool = &mut ctx.accounts.pool;

        if user.balance < amount {
            return Err(ProgramError::InsufficientFunds.into());
        }

        // Deduct the balance from the user and add it to the pool
        user.balance -= amount;
        pool.balance += amount;

        Ok(())
    }

    // Borrowing function with interest
    pub fn borrow(ctx: Context<Borrow>, amount: u64) -> Result<()> {
        let user = &mut ctx.accounts.user;
        let pool = &mut ctx.accounts.pool;

        // Simulate interest rate of 5% for borrowing
        let interest = (amount as f64 * 0.05) as u64;
        let total_amount = amount + interest;

        if pool.balance < total_amount {
            return Err(ProgramError::InsufficientFunds.into());
        }

        // Deduct from the pool and credit the user
        pool.balance -= total_amount;
        user.balance += amount;

        Ok(())
    }

    // Withdraw funds (lender can withdraw balance)
    pub fn withdraw(ctx: Context<Withdraw>, amount: u64) -> Result<()> {
        let user = &mut ctx.accounts.user;
        let pool = &mut ctx.accounts.pool;

        if user.balance < amount {
            return Err(ProgramError::InsufficientFunds.into());
        }

        // Deduct from user and add back to the pool
        user.balance -= amount;
        pool.balance += amount;

        Ok(())
    }
}

#[derive(Accounts)]
pub struct InitializePool<'info> {
    #[account(init, payer = user, space = 8 + 8)]
    pub pool: Account<'info, Pool>,
    #[account(mut)]
    pub user: Signer<'info>,
    pub system_program: Program<'info, System>,
}

#[derive(Accounts)]
pub struct Lend<'info> {
    #[account(mut)]
    pub user: Signer<'info>,
    #[account(mut)]
    pub pool: Account<'info, Pool>,
}

#[derive(Accounts)]
pub struct Borrow<'info> {
    #[account(mut)]
    pub user: Signer<'info>,
    #[account(mut)]
    pub pool: Account<'info, Pool>,
}

#[derive(Accounts)]
pub struct Withdraw<'info> {
    #[account(mut)]
    pub user: Signer<'info>,
    #[account(mut)]
    pub pool: Account<'info, Pool>,
}

#[account]
pub struct Pool {
    pub balance: u64,
}