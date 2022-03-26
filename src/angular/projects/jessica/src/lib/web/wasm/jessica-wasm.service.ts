import { Injectable } from '@angular/core';
import { jessica } from '../../../assets/jessica-web';

@Injectable({
  providedIn: 'root'
})
export class JessicaWasmService {
  private instance?: any;

  constructor() {
    jessica().then(async (instance: any) => {
      this.instance = instance;
    });
  }

  wasm(): any {
    return this.instance;
  }
}
