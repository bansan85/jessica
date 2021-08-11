import { Injectable } from '@angular/core';
import * as Module from '../../../assets/jessica-web';

@Injectable({
  providedIn: 'root'
})
export class JessicaWasmService {
  private instance?: any;

  constructor() {
    Module.default().then(async (instance: any) => {
      this.instance = instance;
    });
  }

  wasm(): any {
    return this.instance;
  }
}
