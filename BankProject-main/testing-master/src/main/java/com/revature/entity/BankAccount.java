package com.revature.entity;

import java.io.Serializable;
import java.util.Objects;

public class BankAccount implements Serializable {

    private int accountNumber;
    private final int routingNumber = 123456789;
    private double balance;

    public BankAccount() {}

    public BankAccount(int accountNumber, double balance) {
        this.accountNumber = accountNumber;
        this.balance = balance;

    }

    public int getAccountNumber() {
        return accountNumber;
    }

    public int getRoutingNumber() {
        return routingNumber;
    }

    public double getBalance() {
        return balance;
    }

    public double deposit(double amount) {
        return balance += amount;
    }

    public double withdraw(double amount) {
        if ((balance - amount) >= 0) {
            balance -= amount;
            return  balance;
        }
        return 0;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        BankAccount that = (BankAccount) o;
        return accountNumber == that.accountNumber && routingNumber == that.routingNumber && Double.compare(balance, that.balance) == 0;
    }

    @Override
    public int hashCode() {
        return Objects.hash(accountNumber, routingNumber, balance);
    }

    @Override
    public String toString() {
        return "BankAccount {" +
                "accountNumber =" + accountNumber +
                ", routingNumber =" + routingNumber +
                ", balance=" + balance +
                '}';
    }
}