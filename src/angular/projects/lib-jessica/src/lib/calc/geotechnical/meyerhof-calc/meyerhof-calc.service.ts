/* eslint-disable max-len */
import { Injectable } from '@angular/core';
import { MeyerhofForm } from '../../../data/geotechnical/meyerhof/meyerhof-form';
import { JessicaWasmService } from '../../../web/wasm/jessica-wasm.service';
import { MeyerhofResult } from './meyerhof-result';
/* eslint-enable max-len */

@Injectable({
  providedIn: 'root'
})
export class MeyerhofCalcService {
  constructor(private wasm: JessicaWasmService) {}

  compute(newItem: MeyerhofForm): MeyerhofResult {
    const ttt: any = this.wasm.wasm();
    const foundation: any = new ttt.FoundationStrip().setB(
      Number(newItem.foundation.width)
    );
    const load: any = new ttt.VerticalEccentric()
      .setV(Number(newItem.load.load))
      .setE(Number(newItem.load.eccentric));
    const calc: any = new ttt.MeyerhofShallowFoundation(load, foundation);

    return { qref: calc.getQref(), b_: calc.getB_() };
  }
}
