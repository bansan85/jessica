import { ComponentFixture, TestBed } from '@angular/core/testing';

import { MeyerhofCalcComponent } from './meyerhof-calc.component';

describe('MeyerhofCalcComponent', () => {
  let component: MeyerhofCalcComponent;
  let fixture: ComponentFixture<MeyerhofCalcComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [MeyerhofCalcComponent]
    }).compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(MeyerhofCalcComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
