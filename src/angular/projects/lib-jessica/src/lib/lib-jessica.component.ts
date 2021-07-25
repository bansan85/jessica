import { Component, OnInit } from '@angular/core';
import * as Module from './jessica-web';

@Component({
  selector: 'lib-lib-jessica',
  template: ` <p>lib-jessica works!</p> `,
  styles: [],
})
export class LibJessicaComponent implements OnInit {
  private instance?: any;
  constructor() {
    Module.default().then(async (instance: any) => {
      this.instance = instance;
      const vert1 = new this.instance.VerticalEccentricRaw();
      const vert2 = vert1.setE(0.2);
      console.log('getE: ' + vert2.getE());
    });
  }

  ngOnInit(): void {}
}
