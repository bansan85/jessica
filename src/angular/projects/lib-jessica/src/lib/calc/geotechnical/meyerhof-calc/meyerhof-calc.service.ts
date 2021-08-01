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
    const lo: any = this.wasm.log();
    const foundation: any = new ttt.FoundationStripRaw().setB(
      Number(newItem.foundation.width)
    );
    const load: any = new ttt.VerticalEccentricRaw()
      .setV(Number(newItem.load.load))
      .setE(Number(newItem.load.eccentric));
    const calc: any = new ttt.MeyerhofShallowFoundationDeco(
      lo,
      lo,
      load,
      foundation
    );

    return { qref: calc.getQref(), b_: calc.getB_() };
  }
}
