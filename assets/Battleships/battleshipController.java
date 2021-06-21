/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Battleships;

/**
 *
 * @author Ryan
 */

import java.util.ArrayList;

public class battleshipController {
    public static void main(String[] args) {
        battleshipView view = new battleshipView();
        battleshipModel mod = new battleshipModel();

        // Example of querying for results using the view and model
        int year = view.getUserInputYear();
        ArrayList<Ship> results = mod.queryShipsByYear(year);
        view.displayShips(results);

        // Example of inserting a new record into the database using the view and model
        //String className = view.getUserInputClass();
        //String countryName = view.getUserInputCountry();
        //System.out.println("Inserting " + className + " " + countryName + ".");
        //mod.insertNewClass(className, countryName);
    }
}