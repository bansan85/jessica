import { Component, OnInit } from '@angular/core';
import * as Module from './jessica';

@Component({
  selector: 'app-main',
  templateUrl: './main.component.html',
  styleUrls: ['./main.component.css'],
})
export class MainComponent implements OnInit {
  private instance?: any;
  constructor() {
    Module.default().then(async (instance: any) => {
      this.instance = instance;
      const vert1 = new this.instance.VerticalEccentricRaw();
      const vert2 = vert1.setE(0.2);
      console.log('rezareza' + vert2.getE() + 'fdnskqjl');
    });
  }

  ngOnInit(): void {}
}
