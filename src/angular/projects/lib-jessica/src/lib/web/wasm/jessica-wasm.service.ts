import { Injectable } from '@angular/core';
import * as Module from '../../jessica-web';

@Injectable({
  providedIn: 'root'
})
export class JessicaWasmService {
  private instance?: any;
  private logger?: any;

  constructor() {
    Module.default().then(async (instance: any) => {
      this.instance = instance;
      this.logger = new this.instance.SpdlogStdoutMt('log');
    });
  }

  wasm(): any {
    return this.instance;
  }

  log(): any {
    return this.logger;
  }
}
