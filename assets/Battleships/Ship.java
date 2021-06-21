/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Battleships;

public class Ship {
	
	private String name;
	private String shipClass;
	private int launched;
	
	public Ship (String n, String c, int l) {
		setName(n);
		setShipClass(c);
		setLaunched(l);
	}
	
	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getShipClass() {
		return shipClass;
	}

	public void setShipClass(String shipClass) {
		this.shipClass = shipClass;
	}

	public int getLaunched() {
		return launched;
        }

	public void setLaunched(int launched) {
		this.launched = launched;
	}
	
}