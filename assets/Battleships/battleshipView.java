/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Ryan
 */
package Battleships;

import java.util.ArrayList;
import java.util.Scanner;

public class battleshipView {
    Scanner kb;

    public battleshipView() {
        kb = new Scanner(System.in);
    }

    public int getUserInputYear() {
        System.out.println("Enter the year that you would like to see: ");
        int launched = kb.nextInt();
        kb.nextLine();
        return launched;
    }

    public String getUserInputClass() {
        System.out.println("Enter the name of the new class to be inserted: ");
        String className = kb.nextLine();
        return className;
    }

    public String getUserInputCountry() {
        System.out.println("Enter the country of the new class: ");
        String country = kb.nextLine();
        return country;
    }

    public void displayShips(ArrayList<Ship> ships) {
        int numShips = ships.size();
        for (int i = 0; i < numShips; i++) {
            Ship currShip = ships.get(i);
            System.out.println("Ship name: " + currShip.getName());
            System.out.println("Ship class: " + currShip.getShipClass());
            System.out.println("Year launched: " + currShip.getLaunched());
            System.out.println();
        }
        System.out.println("End of data.");
    }
}