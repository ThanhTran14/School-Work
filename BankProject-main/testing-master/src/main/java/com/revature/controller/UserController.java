package com.revature.controller;

import com.revature.entity.User;
import com.revature.exception.LoginFail;
import com.revature.exception.ValidateFail;
import com.revature.service.UserService;

import java.util.Map;
import java.util.Scanner;

public class UserController {

    private Scanner scanner;
    private UserService userService;

    public UserController(Scanner scanner, UserService userService) {
        this.scanner = scanner;
        this.userService = userService;
    }

    public void promptUserForService(Map<String,String> controlMap) {

        System.out.println("1. Make a new account");
        System.out.println("2. Login");
        System.out.println(("q. quit"));
        System.out.print("What would you like to do? ");

        try {
            String userAction = scanner.nextLine();
            switch (userAction) {
                case "1":
                    registerUser();
                    break;

                case "2":
                    controlMap.put("User", login().getUsername());
                    System.out.printf("Banking stuff for %s can happen here!\n\n", controlMap.get("User"));
                    System.out.println("d. Deposit into account");
                    System.out.println("w. Withdraw money from account");
                    System.out.println("o. Logout");
                    System.out.println("What would you like to do? ");
                    String userAction2 = scanner.nextLine();
                    switch (userAction2) {
                        case "d":
                            System.out.println("Deposit");
                            break;

                        case "w":
                            System.out.println("Withdraw");
                            break;

                        case "o":

                            logout(controlMap);
                            System.out.println("\nYou have logged out\n");
                            break;
                    }
                    break;





                case "q":
                    System.out.println("GoodBye!");
                    controlMap.put("Continue Loop", "false");
            }
        } catch (LoginFail e) {
            System.out.println(e.getMessage());
        }
    }

    public void registerUser() {
        try {
            User newCredentials = getUserCredentials();
            User newUser = userService.validate(newCredentials);
            System.out.printf("New account created:  %s  \n", newUser);
        } catch (ValidateFail e) {
            System.out.println("Sorry Username has already taken. Please try again \n");
        }
    }

    public User login() {
        return userService.checkLogin(getUserCredentials());
    }

    public void logout(Map<String, String> controlMap) {
        controlMap.remove("User");
    }

    public User getUserCredentials() {
        String newUsername;
        String newPassword;
        System.out.print("Please enter a username: ");
        newUsername = scanner.nextLine();
        System.out.print("Please enter a password: ");
        newPassword = scanner.nextLine();
        return new User(newUsername, newPassword);
    }
}